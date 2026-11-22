/*
 * sseqrender.c - offline renderer for Nintendo DS SDAT sequences (SSEQ / SSAR)
 * to stereo PCM16 at the DS mixer rate, for the Sonic Rush Dreamcast port.
 *
 * Written from scratch for this project from the published SDAT format notes
 * (Kiwi.ds "NDS Sound Data" spec, GBATEK sound chapter) and from the
 * behaviour of the NitroSDK ARM7 sound driver (SND_seq / SND_exChannel /
 * SND_util). No third-party code is copied. Driver tables that are game
 * library data (decibel-square table, channel volume table, attack table,
 * LFO sine table, channel allocation order, ARM9 NNS decibel table) are NOT embedded: sdat2dc.py
 * extracts them from the owner's ROM (arm7.bin / arm9.bin) and passes them
 * in with --tables.
 *
 * Model (see docs/dreamcast/formats/SOUND.md):
 *   - one "frame" = 2728*64 ARM7 cycles = 191.96 Hz = exactly 170.5 output
 *     samples at 33513982/1024 = 32728.5 Hz (written as 32728 Hz).
 *   - per frame: apply channel registers computed last frame, run the
 *     sequencer (tempo counter / 240), update track->channel parameters,
 *     run envelopes / LFO / sweep and compute new registers, then mix.
 *   - 16 hardware channels, PCM 0-15, PSG 8-13, noise 14-15, NitroSDK
 *     allocation order and priority stealing.
 *   - sampling: no interpolation (zero-order hold, like the DS) by default.
 *   - volume stages as on the DS: ARM9 NNSi_SndPlayerMain (decompiled,
 *     src/libs/NitroSystem/snd) once per 60 Hz frame turns the NNS volumes into
 *     the driver player volume with the LINEAR dB table data_020725e0
 *     (SNDi_DecibelTable, 200*log10):
 *       extFader = clamp(db[fader>>8] + db[volume1 = SEQ/SSAR info volume]
 *                        + db[volume2 = NNS_SndPlayerSetVolume, 127]
 *                        + db[player volume, 127], -723, 0)
 *     ARM7 driver: track = dbsq[track vol] + dbsq[expression] + dbsq[seq master
 *     volume (0xC2)], channel = dbsq[velocity] + envelope (sustain dbsq) +
 *     track + extFader (+ LFO), SQUARED table (400*log10); register
 *     = SND_CalcChannelVolume; mixer vol/128 >> shift, pan (128-p)/128, p/128,
 *     master 127/128.
 *
 * Usage:
 *   sseqrender render --sdat F --tables T --seq FILEID --start OFF
 *                     --bank FILEID --swar a,b,c,d [--vol V] [--ppr P]
 *                     [--mask M] [--max-sec S] [--interp] [--gain-tl N] --out OUT.wav
 *       --gain-tl N: port mix gain, 2^(-N/16) (N AICA TL steps of 0.375 dB),
 *       applied after the DS master volume (sdat2dc.py MIX_GAIN_TL; the
 *       runtime applies the same gain to the effects, from the .srb header).
 *       prints one JSON object (loop points, stats) to stdout.
 *   sseqrender strm --sdat F --file FILEID --out OUT.wav
 *       decodes a STRM (PCM8 / PCM16 / IMA-ADPCM, block-interleaved channels)
 *       to PCM16 at its native rate; prints JSON (rate, channels, loop).
 *   sseqrender aicadec IN.wav OUT.wav
 *       decodes a KOS wav2adpcm output (fmt 0x14 mono / non-interleaved
 *       stereo, fmt 0x20 interleaved stereo) back to PCM16 with the AICA
 *       Yamaha ADPCM decoder model (no high-pass), for round-trip checks.
 *   sseqrender swar --sdat F --file FILEID --outdir DIR
 *       decodes every SWAV of a SWAR to DIR/NNNN.wav (mono PCM16 at the
 *       wave's native rate); prints JSON list of wave info.
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

#define FRAME_SAMPLES_X2 341 /* 170.5 samples per frame */
#define OUT_RATE 32728
#define AMPL_MIN (-92544)
#define LOOP_TAIL_EXACT 1024 /* = JOIN_M of the ADPCM encoder */
#define HOLD_FADE_NNS_FRAMES 10 /* held-SFX stop: NNS fade length in 60 Hz frames (assumption, ~0.17 s) */

static int16_t tblDecibelSq[128];
static int16_t tblDecibel[128];   /* ARM9 SNDi_DecibelTable = data_020725e0 (NNS player volume) */
static uint8_t tblVolume[724];
static uint8_t tblAttack[19];
static uint8_t tblSine[33];
static uint8_t tblOrder[16];

static const int IMA_STEP[89] = {
    7, 8, 9, 10, 11, 12, 13, 14, 16, 17, 19, 21, 23, 25, 28, 31, 34, 37, 41, 45, 50, 55, 60, 66, 73,
    80, 88, 97, 107, 118, 130, 143, 157, 173, 190, 209, 230, 253, 279, 307, 337, 371, 408, 449, 494,
    544, 598, 658, 724, 796, 876, 963, 1060, 1166, 1282, 1411, 1552, 1707, 1878, 2066, 2272, 2499,
    2749, 3024, 3327, 3660, 4026, 4428, 4871, 5358, 5894, 6484, 7132, 7845, 8630, 9493, 10442,
    11487, 12635, 13899, 15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794, 32767};
static const int IMA_INDEX[8] = {-1, -1, -1, -1, 2, 4, 6, 8};

static void die(const char *m) { fprintf(stderr, "sseqrender: %s\n", m); exit(1); }
static uint32_t rd32(const uint8_t *p) { return p[0] | (p[1] << 8) | (p[2] << 16) | ((uint32_t)p[3] << 24); }
static uint16_t rd16(const uint8_t *p) { return p[0] | (p[1] << 8); }

/* ---------------- SDAT file access ---------------- */
static uint8_t *sdat; static long sdatLen; static uint32_t fatOff, fatCount;
static void load_sdat(const char *path) {
    FILE *f = fopen(path, "rb"); if (!f) die("cannot open sdat");
    fseek(f, 0, SEEK_END); sdatLen = ftell(f); fseek(f, 0, SEEK_SET);
    sdat = malloc(sdatLen); if (fread(sdat, 1, sdatLen, f) != (size_t)sdatLen) die("read sdat"); fclose(f);
    if (memcmp(sdat, "SDAT", 4)) die("not SDAT");
    fatOff = rd32(sdat + 0x20); fatCount = rd32(sdat + fatOff + 8);
}
static const uint8_t *sdat_file(int id, uint32_t *size) {
    if (id < 0 || (uint32_t)id >= fatCount) return NULL;
    const uint8_t *e = sdat + fatOff + 12 + id * 16;
    if (size) *size = rd32(e + 4);
    return sdat + rd32(e);
}

static void load_tables(const char *path) {
    FILE *f = fopen(path, "rb"); if (!f) die("cannot open tables");
    uint8_t buf[256 + 724 + 19 + 33 + 16 + 256];
    if (fread(buf, 1, sizeof buf, f) != sizeof buf) die("tables file too short (re-run sdat2dc.py: needs the NNS decibel table)");
    fclose(f);
    for (int i = 0; i < 128; i++) tblDecibelSq[i] = (int16_t)rd16(buf + 2 * i);
    for (int i = 0; i < 128; i++) tblDecibel[i] = (int16_t)rd16(buf + 1048 + 2 * i);
    memcpy(tblVolume, buf + 256, 724); memcpy(tblAttack, buf + 980, 19);
    memcpy(tblSine, buf + 999, 33); memcpy(tblOrder, buf + 1032, 16);
}

/* ---------------- waves ---------------- */
typedef struct { int16_t *pcm; int len, loopStart, loop, timer, rate, fmt, ok; } Wave;

static int decode_swav(const uint8_t *w, uint32_t avail, Wave *out) {
    int fmt = w[0], loop = w[1];
    int rate = rd16(w + 2), timer = rd16(w + 4);
    uint32_t ls = rd16(w + 6), ll = rd32(w + 8);
    const uint8_t *d = w + 12; uint32_t bytes = (ls + ll) * 4;
    if (bytes + 12 > avail) bytes = avail > 12 ? avail - 12 : 0;
    int n, lsS;
    int16_t *p;
    if (fmt == 0) { n = bytes; lsS = ls * 4; p = malloc(sizeof(int16_t) * (n + 1));
        for (int i = 0; i < n; i++) p[i] = (int16_t)((int8_t)d[i] * 256);
    } else if (fmt == 1) { n = bytes / 2; lsS = ls * 2; p = malloc(sizeof(int16_t) * (n + 1));
        for (int i = 0; i < n; i++) p[i] = (int16_t)rd16(d + 2 * i);
    } else if (fmt == 2) {
        if (bytes < 4) return 0;
        n = (bytes - 4) * 2; lsS = ls ? (ls - 1) * 8 : 0; p = malloc(sizeof(int16_t) * (n + 1));
        int s = (int16_t)rd16(d), idx = d[2] & 0x7F; if (idx > 88) idx = 88;
        for (uint32_t i = 0; i < bytes - 4; i++) {
            for (int h = 0; h < 2; h++) {
                int nib = h ? d[4 + i] >> 4 : d[4 + i] & 15, st = IMA_STEP[idx];
                int diff = st >> 3;
                if (nib & 1) diff += st >> 2;
                if (nib & 2) diff += st >> 1;
                if (nib & 4) diff += st;
                if (nib & 8) { s -= diff; if (s < -0x7FFF) s = -0x7FFF; }
                else { s += diff; if (s > 0x7FFF) s = 0x7FFF; }
                idx += IMA_INDEX[nib & 7]; if (idx < 0) idx = 0; if (idx > 88) idx = 88;
                p[2 * i + h] = (int16_t)s;
            }
        }
    } else return 0;
    if (lsS > n) lsS = n;
    out->pcm = p; out->len = n; out->loopStart = lsS; out->loop = loop && n > lsS;
    out->timer = timer ? timer : 1; out->rate = rate; out->fmt = fmt; out->ok = n > 0;
    return out->ok;
}

typedef struct { const uint8_t *data; uint32_t size; int count; Wave *waves; } Swar;
static Swar swars[4];
static void load_swar(int slot, int fileId) {
    swars[slot].data = NULL;
    if (fileId < 0) return;
    const uint8_t *d = sdat_file(fileId, &swars[slot].size);
    if (!d || memcmp(d, "SWAR", 4)) return;
    swars[slot].data = d; swars[slot].count = rd32(d + 0x38);
    swars[slot].waves = calloc(swars[slot].count + 1, sizeof(Wave));
}
static Wave *get_wave(int slot, int idx) {
    if (slot < 0 || slot > 3 || !swars[slot].data || idx >= swars[slot].count) return NULL;
    Wave *w = &swars[slot].waves[idx];
    if (!w->ok && !w->pcm) {
        uint32_t off = rd32(swars[slot].data + 0x3C + 4 * idx);
        uint32_t end = (idx + 1 < swars[slot].count) ? rd32(swars[slot].data + 0x40 + 4 * idx) : swars[slot].size;
        if (off >= swars[slot].size || end <= off) { w->pcm = (int16_t *)1; return NULL; }
        if (!decode_swav(swars[slot].data + off, end - off, w)) { w->pcm = (int16_t *)1; return NULL; }
    }
    return w->ok ? w : NULL;
}

/* ---------------- bank ---------------- */
static const uint8_t *bank; static uint32_t bankSize;
typedef struct { int type; int wave, swar, duty, rootKey, attack, decay, sustain, release, pan; } Inst;
static void parse_param(const uint8_t *p, int type, Inst *o) {
    o->type = type; o->wave = rd16(p); o->swar = rd16(p + 2); o->duty = rd16(p);
    o->rootKey = p[4]; o->attack = p[5]; o->decay = p[6]; o->sustain = p[7]; o->release = p[8]; o->pan = p[9];
}
static int read_inst(int prg, int key, Inst *o) {
    if (!bank || prg < 0) return 0;
    uint32_t cnt = rd32(bank + 0x38);
    if ((uint32_t)prg >= cnt) return 0;
    uint32_t v = rd32(bank + 0x3C + 4 * prg);
    int type = v & 0xFF; uint32_t off = v >> 8;
    if (off + 12 > bankSize) return 0;
    const uint8_t *d = bank + off;
    switch (type) {
    case 1: case 2: case 3: case 4: case 5: parse_param(d, type, o); return 1;
    case 16: {
        int lo = d[0], hi = d[1];
        if (key < lo || key > hi) return 0;
        const uint8_t *e = d + 2 + (key - lo) * 12;
        parse_param(e + 2, e[0], o); return 1;
    }
    case 17: {
        int r = 0;
        while (key > d[r]) { r++; if (r >= 8) return 0; }
        const uint8_t *e = d + 8 + r * 12;
        parse_param(e + 2, e[0], o); return 1;
    }
    default: return 0;
    }
}

/* ---------------- channels ---------------- */
enum { ENV_ATTACK, ENV_DECAY, ENV_SUSTAIN, ENV_RELEASE };
typedef struct { int target, speed, depth, range, delay; } LfoParam;
struct Track;
typedef struct {
    /* ExChannel (driver) state */
    int active, start, type; /* type 0 pcm 1 psg 2 noise */
    int prio; struct Track *track; int nextInTrack;
    Wave *wave; int duty; int baseTimer;
    int key, rootKey, velocity, initPan;
    int envStatus, envAttn, envAttack, envDecay, envSustain, envRelease;
    int length;
    int sweepPitch, sweepLength, sweepCounter, autoSweep;
    int userDecay, userDecay2, userPitch, userPan, panRange;
    LfoParam lfo; int lfoCounter, lfoDelayCnt;
    int volume; /* last computed volume register (vol | shift<<8) */
    int regTimer, regPan;
    int stopReq; /* stop at next register update */
    long startTick; long logIdx;
} ExChan;
typedef struct {
    int active; int type; Wave *wave; int duty; uint32_t lfsr; int noiseOut;
    uint64_t pos; int vol, shift, pan, timer;
} HwChan;
static ExChan ch[16]; static HwChan hw[16];

#define SNAP_N 28
typedef struct Track {
    int alloc, active; uint32_t pc;
    int wait, noteWait, noteFinishWait, tie, porta, portaKey, portaTime, sweepPitch, muted;
    int vol, expr, pan, panRange, transpose, bend, bendRange, prio, prg;
    int attack, decay, sustain, release;
    LfoParam mod; int cmp;
    uint32_t stack[3]; int loopCnt[3]; int depth;
    int chanHead; /* index of first channel, -1 none */
    int *firstTick; /* per data byte: first tick executed */
    int32_t (*snap)[SNAP_N]; /* per data byte: parameter state at first execution */
    int stateDiffers; /* loop target state != state at loop jump (first pass differs) */
    long loopStartTick, loopEndTick; int looped;
} Track;

static const uint8_t *seqData; static uint32_t seqLen; /* data block (offsets relative to this) */
static Track tracks[16]; static int trackMask;
static int playerVolume = 127, playerFader = 0, playerPrio = 64, tempo = 120, tempoCounter = 240;
static int16_t vars[32];

/* ---- ARM9 NNS player (decompiled NNSi_SndPlayerMain, NNSi_SndFader*) ---- */
typedef struct { int32_t origin, target, counter, frame; } NNSSndFader;
static int32_t nns_fader_get(const NNSSndFader *f) {
    if (f->counter >= f->frame) return f->target;
    return f->origin + (f->target - f->origin) * f->counter / f->frame;
}
static void nns_fader_set(NNSSndFader *f, int32_t target, int32_t frame) {
    f->origin = nns_fader_get(f); f->target = target; f->frame = frame; f->counter = 0;
}
static struct {
    NNSSndFader fader;
    int status;            /* 1 playing, 2 fading out (NNSi_SndPlayerStopSeq) */
    int volume;            /* last value sent with SND_SetPlayerVolume */
    int volume1, volume2;  /* SetInitialVolume (SEQ/SSAR info), SetVolume (gBGMVolume / gSEVolume) */
    int playerVolume;      /* NNSSndPlayer volume (NNS_SndPlayerSetPlayerVolume, default 127) */
} nns;
static void player_stop_all(void);
/* NNSi_SndPlayerMain for the one player, once per 60 Hz frame */
static void nns_player_main(void) {
    int vol, v0, v1, v2;
    if (nns.fader.counter < nns.fader.frame) nns.fader.counter++;   /* NNSi_SndFaderUpdate */
    v2 = tblDecibel[nns.volume2 & 127];
    v1 = tblDecibel[nns.volume1 & 127];
    v0 = tblDecibel[nns.playerVolume & 127];
    vol = tblDecibel[(nns_fader_get(&nns.fader) >> 8) & 127];
    vol += v1 + v2 + v0;
    if (vol < -723) vol = -723; else if (vol > 0) vol = 0;
    if (vol != nns.volume) { playerFader = vol; nns.volume = vol; }  /* SND_SetPlayerVolume */
    if (nns.status == 2 && nns.fader.counter >= nns.fader.frame) {  /* ForceStopSeq */
        playerFader = -723; player_stop_all(); nns.status = 0;
    }
}
static uint32_t rnd = 0x12345678;
static long tickNo = 0, frameNo = 0;
static int stealCount = 0, maxVoices = 0, chanMaskAllowed = 0xFFFF, instMissing = 0, waveMissing = 0, allocFail = 0;

typedef struct { long startTick, startFrame, endFrame; int track, key; } NoteLog;
static NoteLog *notes; static long nNotes, capNotes;

static int calc_random(void) { rnd = rnd * 1664525u + 1013904223u; return rnd >> 16; }
static int sin_idx(int x) {
    if (x < 0x20) return tblSine[x];
    if (x < 0x40) return tblSine[0x40 - x];
    if (x < 0x60) return -tblSine[x - 0x40];
    return -tblSine[0x20 - (x - 0x60)];
}
static int attack_coef(int a) { return a < 109 ? 255 - a : tblAttack[127 - a]; }
static int decay_coef(int v) {
    if (v == 127) return 0xFFFF;
    if (v == 126) return 0x3C00;
    if (v < 50) return v * 2 + 1;
    return 0x1E00 / (126 - v);
}
static int calc_timer(int timer, int pitch) {
    int octave = 0, p = -pitch;
    while (p < 0) { octave--; p += 768; }
    while (p >= 768) { octave++; p -= 768; }
    /* NitroSDK pitch table entry: 65536*(2^(p/768)-1), rounded */
    uint64_t r = (uint64_t)llround(65536.0 * (pow(2.0, p / 768.0) - 1.0));
    r += 0x10000; r *= (uint64_t)timer;
    int shift = octave - 16;
    if (shift <= 0) r >>= -shift;
    else if (shift < 32) { if (r & (~0ULL << (32 - shift))) return 0xFFFF; r <<= shift; }
    else return 0xFFFF;
    if (r < 0x10) r = 0x10; else if (r > 0xFFFF) r = 0xFFFF;
    return (int)r;
}
static int calc_volume(int dB) {
    if (dB < -723) dB = -723; else if (dB > 0) dB = 0;
    int v = tblVolume[dB + 723], s = dB < -240 ? 3 : dB < -120 ? 2 : dB < -60 ? 1 : 0;
    return v | (s << 8);
}
static const int SHIFT_TAB[4] = {0, 1, 2, 4};

static void lfo_init(LfoParam *l) { l->target = 0; l->depth = 0; l->range = 1; l->speed = 16; l->delay = 0; }

/* remove channel from its track list */
static void track_unlink(int c) {
    Track *t = ch[c].track; if (!t) return;
    int *pp = &t->chanHead;
    while (*pp >= 0) { if (*pp == c) { *pp = ch[c].nextInTrack; break; } pp = &ch[*pp].nextInTrack; }
    ch[c].track = NULL; ch[c].nextInTrack = -1;
}
static void chan_finished(int c) { /* FINISH callback */
    if (ch[c].logIdx >= 0) notes[ch[c].logIdx].endFrame = frameNo;
    ch[c].logIdx = -1;
    track_unlink(c); ch[c].prio = 0; ch[c].volume = 0; ch[c].active = 0;
}
static int vol_cmp(const ExChan *a, const ExChan *b) {
    int va = (a->volume & 0xFF) << 4, vb = (b->volume & 0xFF) << 4;
    va >>= SHIFT_TAB[(a->volume >> 8) & 3]; vb >>= SHIFT_TAB[(b->volume >> 8) & 3];
    if (va != vb) return va < vb ? 1 : -1;
    return 0;
}
static int alloc_chan(int mask, int prio) {
    int best = -1;
    for (int i = 0; i < 16; i++) {
        int c = tblOrder[i];
        if (!(mask & (1 << c))) continue;
        if (best < 0) { best = c; continue; }
        if (ch[c].prio > ch[best].prio) continue;
        if (ch[c].prio != ch[best].prio || vol_cmp(&ch[best], &ch[c]) < 0) best = c;
    }
    if (best < 0) return -1;
    if (prio < ch[best].prio) return -1;
    if (ch[best].active) { stealCount++; if (ch[best].logIdx >= 0) notes[ch[best].logIdx].endFrame = frameNo; }
    track_unlink(best);
    ExChan *e = &ch[best];
    memset(e, 0, sizeof *e);
    e->prio = prio; e->volume = 127; e->autoSweep = 1; e->key = 60; e->rootKey = 60; e->velocity = 127;
    e->panRange = 127; e->envAttack = attack_coef(127); e->envDecay = decay_coef(127);
    e->envSustain = 127; e->envRelease = decay_coef(127); lfo_init(&e->lfo);
    e->nextInTrack = -1; e->logIdx = -1;
    hw[best].active = 0; e->stopReq = 0;
    return best;
}
static int note_on(int c, int key, int vel, int length, const Inst *in) {
    ExChan *e = &ch[c];
    int release = in->release;
    if (release == 0xFF) { length = -1; release = 0; }
    switch (in->type) {
    case 1: case 4: {
        Wave *w = get_wave(in->swar, in->wave);
        if (!w) return 0;
        e->type = 0; e->wave = w; e->baseTimer = w->timer; break;
    }
    case 2: if (c < 8 || c > 13) return 0; e->type = 1; e->duty = in->duty & 7; e->baseTimer = 8006; break;
    case 3: if (c < 14) return 0; e->type = 2; e->baseTimer = 8006; break;
    default: return 0;
    }
    e->envAttn = AMPL_MIN; e->envStatus = ENV_ATTACK; e->length = length;
    e->sweepPitch = 0; e->sweepLength = 0; e->sweepCounter = 0;
    e->lfoCounter = 0; e->lfoDelayCnt = 0;
    e->start = 1; e->active = 1;
    e->key = key; e->rootKey = in->rootKey; e->velocity = vel;
    e->envAttack = attack_coef(in->attack); e->envDecay = decay_coef(in->decay);
    e->envSustain = in->sustain; e->envRelease = decay_coef(release);
    e->initPan = in->pan - 64;
    return 1;
}
static void release_chan(int c, int rel) {
    if (!ch[c].active) return;
    if (rel >= 0) ch[c].envRelease = decay_coef(rel & 0xFF);
    ch[c].prio = 1; ch[c].envStatus = ENV_RELEASE;
}

/* ---------------- sequencer ---------------- */
static void track_init(Track *t) {
    int *ft = t->firstTick; int32_t (*sn)[SNAP_N] = t->snap; memset(t, 0, sizeof *t); t->firstTick = ft; t->snap = sn;
    t->noteWait = 1; t->cmp = 1; t->prio = 64; t->vol = 127; t->expr = 127; t->panRange = 127; t->bendRange = 2;
    t->portaKey = 60; t->attack = t->decay = t->sustain = t->release = 0xFF; lfo_init(&t->mod);
    t->chanHead = -1; t->loopStartTick = t->loopEndTick = -1;
}
static void track_release_all(Track *t, int rel) {
    for (int c = t->chanHead; c >= 0; c = ch[c].nextInTrack) release_chan(c, rel);
}
static void track_free_chans(Track *t) {
    while (t->chanHead >= 0) { int c = t->chanHead; t->chanHead = ch[c].nextInTrack; ch[c].track = NULL; ch[c].nextInTrack = -1; }
}
static void track_stop(Track *t) { track_release_all(t, -1); track_free_chans(t); t->active = 0; }

static int rd8s(Track *t) { return t->pc < seqLen ? seqData[t->pc++] : 0xFF; }
static int read_arg(Track *t, int type) {
    int v = 0;
    switch (type) {
    case 0: v = rd8s(t); break;                                         /* u8 */
    case 1: v = rd8s(t); v |= rd8s(t) << 8; v = (int16_t)v; break;     /* s16 */
    case 2: { int b, n = 0; do { b = rd8s(t); v = (v << 7) | (b & 0x7F); } while ((b & 0x80) && ++n < 4); break; }
    case 3: { int lo = rd8s(t); lo |= rd8s(t) << 8; lo = (int16_t)lo; int hi = rd8s(t); hi |= rd8s(t) << 8; hi = (int16_t)hi;
              v = ((calc_random() * (hi - lo + 1)) >> 16) + lo; break; }
    case 4: { int i = rd8s(t); if (i < 32) v = vars[i]; break; }
    }
    return v;
}
static void play_note(Track *t, int key, int vel, int length) {
    int c = -1;
    if (t->tie && t->chanHead >= 0) { c = t->chanHead; ch[c].key = key; ch[c].velocity = vel; }
    if (c < 0) {
        Inst in;
        if (!read_inst(t->prg, key, &in)) { instMissing++; return; }
        int allowed;
        switch (in.type) { case 1: case 4: allowed = 0xFFFF; break; case 2: allowed = 0x3F00; break; case 3: allowed = 0xC000; break; default: return; }
        allowed &= chanMaskAllowed;
        int pr = playerPrio + t->prio; if (pr > 255) pr = 255;
        c = alloc_chan(allowed, pr);
        if (c < 0) { allocFail++; return; }
        if (!note_on(c, key, vel, t->tie ? -1 : length, &in)) { waveMissing++; ch[c].prio = 0; ch[c].active = 0; return; }
        ch[c].track = t; ch[c].nextInTrack = t->chanHead; t->chanHead = c;
        ch[c].startTick = tickNo;
        if (nNotes == capNotes) { capNotes = capNotes ? capNotes * 2 : 4096; notes = realloc(notes, capNotes * sizeof *notes); }
        notes[nNotes].startTick = tickNo; notes[nNotes].startFrame = frameNo; notes[nNotes].endFrame = -1;
        notes[nNotes].track = (int)(t - tracks); notes[nNotes].key = key;
        ch[c].logIdx = nNotes++;
    }
    ExChan *e = &ch[c];
    if (t->attack != 0xFF) e->envAttack = attack_coef(t->attack);
    if (t->decay != 0xFF) e->envDecay = decay_coef(t->decay);
    if (t->sustain != 0xFF) e->envSustain = t->sustain;
    if (t->release != 0xFF) e->envRelease = decay_coef(t->release);
    e->sweepPitch = t->sweepPitch;
    if (t->porta) e->sweepPitch += (int16_t)((t->portaKey - key) << 6);
    if (t->portaTime == 0) { e->sweepLength = length; e->autoSweep = 0; }
    else { int s = t->portaTime * t->portaTime; s *= e->sweepPitch < 0 ? -e->sweepPitch : e->sweepPitch; e->sweepLength = s >> 11; }
    e->sweepCounter = 0;
}

static void track_start(Track *t, uint32_t off) {
    track_init(t); t->alloc = 1; t->active = 1; t->pc = off;
}

static void snap_fill(const Track *t, int32_t *o) {
    int k = 0;
    o[k++] = t->vol; o[k++] = t->expr; o[k++] = t->pan; o[k++] = t->panRange; o[k++] = t->transpose;
    o[k++] = t->bend; o[k++] = t->bendRange; o[k++] = t->prio; o[k++] = t->prg; o[k++] = t->attack;
    o[k++] = t->decay; o[k++] = t->sustain; o[k++] = t->release; o[k++] = t->mod.target; o[k++] = t->mod.speed;
    o[k++] = t->mod.depth; o[k++] = t->mod.range; o[k++] = t->mod.delay; o[k++] = t->porta; o[k++] = t->portaKey;
    o[k++] = t->portaTime; o[k++] = t->sweepPitch; o[k++] = t->noteWait; o[k++] = t->tie; o[k++] = t->muted;
    o[k++] = tempo; o[k++] = playerVolume; o[k++] = t->depth;
}
static void loop_found(Track *t, uint32_t tgt) {
    int32_t now[SNAP_N]; snap_fill(t, now);
    t->looped = 1; t->loopStartTick = t->firstTick[tgt]; t->loopEndTick = tickNo;
    /* depth differs legitimately for 0xFC loops (stack popped vs not): ignore last field */
    /* portaKey (f19) follows every note; it only matters while portamento is on */
    int32_t then[SNAP_N]; memcpy(then, t->snap[tgt], sizeof then);
    if (!now[18] && !then[18]) now[19] = then[19] = 0;
    t->stateDiffers = memcmp(now, then, sizeof(int32_t) * (SNAP_N - 1)) != 0;
    if (t->stateDiffers && getenv("SSEQ_DEBUG")) {
        fprintf(stderr, "track %d state differs at loop:", (int)(t - tracks));
        for (int k = 0; k < SNAP_N - 1; k++) if (now[k] != then[k]) fprintf(stderr, " f%d %d->%d", k, then[k], now[k]);
        fprintf(stderr, "\n");
    }
}

/* returns 1 if track ended */
static int track_step(int ti) {
    Track *t = &tracks[ti];
    for (int c = t->chanHead; c >= 0; c = ch[c].nextInTrack) {
        if (ch[c].length > 0) ch[c].length--;
        if (!ch[c].autoSweep && ch[c].sweepCounter < ch[c].sweepLength) ch[c].sweepCounter++;
    }
    if (t->noteFinishWait) { if (t->chanHead >= 0) return 0; t->noteFinishWait = 0; }
    if (t->wait > 0) { t->wait--; if (t->wait > 0) return 0; }
    int guard = 0;
    while (t->wait == 0 && !t->noteFinishWait) {
        if (++guard > 20000) return 1; /* runaway loop with no wait: treat as end */
        uint32_t here = t->pc;
        if (here >= seqLen) return 1;
        if (t->firstTick[here] < 0) { t->firstTick[here] = (int)tickNo; snap_fill(t, t->snap[here]); }
        int cmd = rd8s(t), useArg = 0, argType = 0, run = 1;
        if (cmd == 0xA2) { cmd = rd8s(t); run = t->cmp; }
        if (cmd == 0xA0) { cmd = rd8s(t); argType = 3; useArg = 1; }
        if (cmd == 0xA1) { cmd = rd8s(t); argType = 4; useArg = 1; }
        if (cmd < 0x80) {
            int vel = rd8s(t);
            int len = read_arg(t, useArg ? argType : 2);
            int key = cmd + t->transpose;
            if (!run) continue;
            if (key < 0) key = 0; else if (key > 127) key = 127;
            if (!t->muted) play_note(t, key, vel, len > 0 ? len : -1);
            t->portaKey = key;
            if (t->noteWait) { t->wait = len; if (len == 0) t->noteFinishWait = 1; }
            continue;
        }
        switch (cmd & 0xF0) {
        case 0x80: {
            int p = read_arg(t, useArg ? argType : 2);
            if (!run) break;
            if (cmd == 0x80) t->wait = p;
            else if (cmd == 0x81) { if (p < 0x10000) t->prg = p; }
            break;
        }
        case 0x90: {
            if (cmd == 0x93) {
                int tn = rd8s(t); uint32_t off = rd8s(t); off |= rd8s(t) << 8; off |= rd8s(t) << 16;
                if (run && tn < 16 && (trackMask & (1 << tn)) && tn != ti) { track_stop(&tracks[tn]); track_start(&tracks[tn], off); }
            } else if (cmd == 0x94 || cmd == 0x95) {
                uint32_t off = rd8s(t); off |= rd8s(t) << 8; off |= rd8s(t) << 16;
                if (!run) break;
                if (cmd == 0x94) {
                    if (off < seqLen && t->firstTick[off] >= 0 && !t->looped && t->depth == 0) {
                        loop_found(t, off);
                    }
                    t->pc = off;
                } else if (t->depth < 3) { t->stack[t->depth++] = t->pc; t->pc = off; }
            }
            break;
        }
        case 0xC0: case 0xD0: {
            int p = read_arg(t, useArg ? argType : 0);
            if (!run) break;
            switch (cmd) {
            case 0xC0: t->pan = (int8_t)(p - 0x40); break;
            case 0xC1: t->vol = p & 0xFF; break;
            case 0xC2: playerVolume = p & 0xFF; break;
            case 0xC3: t->transpose = (int8_t)p; break;
            case 0xC4: t->bend = (int8_t)p; break;
            case 0xC5: t->bendRange = p & 0xFF; break;
            case 0xC6: t->prio = p & 0xFF; break;
            case 0xC7: t->noteWait = p & 1; break;
            case 0xC8: t->tie = p & 1; track_release_all(t, -1); track_free_chans(t); break;
            case 0xC9: t->portaKey = (p + t->transpose) & 0xFF; t->porta = 1; break;
            case 0xCA: t->mod.depth = p & 0xFF; break;
            case 0xCB: t->mod.speed = p & 0xFF; break;
            case 0xCC: t->mod.target = p & 0xFF; break;
            case 0xCD: t->mod.range = p & 0xFF; break;
            case 0xCE: t->porta = p & 1; break;
            case 0xCF: t->portaTime = p & 0xFF; break;
            case 0xD0: t->attack = p & 0xFF; break;
            case 0xD1: t->decay = p & 0xFF; break;
            case 0xD2: t->sustain = p & 0xFF; break;
            case 0xD3: t->release = p & 0xFF; break;
            case 0xD4: if (t->depth < 3) { t->stack[t->depth] = t->pc; t->loopCnt[t->depth] = p & 0xFF; t->depth++; } break;
            case 0xD5: t->expr = p & 0xFF; break;
            case 0xD6: break;
            case 0xD7: t->muted = p & 1; if (p) { track_release_all(t, -1); } break;
            default: break;
            }
            break;
        }
        case 0xE0: {
            int p = (int16_t)read_arg(t, useArg ? argType : 1);
            if (!run) break;
            if (cmd == 0xE0) t->mod.delay = p & 0xFFFF;
            else if (cmd == 0xE1) tempo = p & 0xFFFF;
            else if (cmd == 0xE3) t->sweepPitch = p;
            break;
        }
        case 0xB0: {
            int vi = rd8s(t);
            int p = (int16_t)read_arg(t, useArg ? argType : 1);
            if (!run || vi >= 32) break;
            int16_t *v = &vars[vi];
            switch (cmd) {
            case 0xB0: *v = p; break;
            case 0xB1: *v += p; break;
            case 0xB2: *v -= p; break;
            case 0xB3: *v *= p; break;
            case 0xB4: if (p) *v /= p; break;
            case 0xB5: if (p >= 0) *v <<= p; else *v >>= -p; break;
            case 0xB6: { int neg = p < 0; if (neg) p = -p; int r = (calc_random() * (p + 1)) >> 16; *v = neg ? -r : r; break; }
            case 0xB8: t->cmp = *v == p; break;
            case 0xB9: t->cmp = *v >= p; break;
            case 0xBA: t->cmp = *v > p; break;
            case 0xBB: t->cmp = *v <= p; break;
            case 0xBC: t->cmp = *v < p; break;
            case 0xBD: t->cmp = *v != p; break;
            }
            break;
        }
        case 0xF0:
            if (!run) break;
            if (cmd == 0xFD) { if (t->depth) { t->depth--; t->pc = t->stack[t->depth]; } }
            else if (cmd == 0xFC) {
                if (!t->depth) break;
                int cnt = t->loopCnt[t->depth - 1];
                if (cnt) { cnt--; if (!cnt) { t->depth--; break; } }
                else if (!t->looped && t->depth == 1) { /* infinite loop block at top level = song loop */
                    uint32_t tgt = t->stack[0];
                    if (tgt < seqLen && t->firstTick[tgt] >= 0) loop_found(t, tgt);
                }
                t->loopCnt[t->depth - 1] = cnt; t->pc = t->stack[t->depth - 1];
            }
            else if (cmd == 0xFE) { rd8s(t); rd8s(t); }
            else if (cmd == 0xFF) return 1;
            break;
        default: break; /* unknown: skip */
        }
    }
    return 0;
}

static int playerActive = 1;
static void player_step_ticks(void) {
    int playing = 0;
    for (int i = 0; i < 16; i++) {
        Track *t = &tracks[i];
        if (!t->active) continue;
        if (track_step(i)) track_stop(t); else playing = 1;
    }
    tickNo++;
    if (!playing) { for (int i = 0; i < 16; i++) if (tracks[i].active) track_stop(&tracks[i]); playerActive = 0; }
}

static void track_update_channels(Track *t) {
    int vol = tblDecibelSq[t->vol & 127] + tblDecibelSq[t->expr & 127] + tblDecibelSq[playerVolume & 127];
    int fader = playerFader;
    int pitch = (t->bend * (t->bendRange << 6)) >> 7;
    int pan = t->pan;
    if (t->panRange != 127) pan = (pan * t->panRange + 0x40) >> 7;
    if (vol < -0x8000) vol = -0x8000;
    if (pan < -128) pan = -128; else if (pan > 127) pan = 127;
    for (int c = t->chanHead; c >= 0; c = ch[c].nextInTrack) {
        ch[c].userDecay2 = fader;
        if (ch[c].envStatus == ENV_RELEASE) continue;
        ch[c].userDecay = vol; ch[c].userPitch = pitch; ch[c].userPan = pan; ch[c].panRange = t->panRange;
        ch[c].lfo = t->mod;
        if (ch[c].length == 0) { ch[c].prio = 1; ch[c].envStatus = ENV_RELEASE; }
    }
}

static void player_stop_all(void) {
    for (int i = 0; i < 16; i++) if (tracks[i].active) track_stop(&tracks[i]);
    playerActive = 0;
}

static void seq_main(void) {
    if (playerActive) {
        int ticks = 0;
        while (tempoCounter >= 240) { tempoCounter -= 240; ticks++; }
        for (int i = 0; i < ticks && playerActive; i++) player_step_ticks();
        tempoCounter += (tempo * 256) >> 8;
    }
    for (int i = 0; i < 16; i++) if (tracks[i].active || tracks[i].chanHead >= 0) track_update_channels(&tracks[i]);
}

static void exchannel_main(void) {
    int voices = 0;
    for (int i = 0; i < 16; i++) {
        ExChan *e = &ch[i];
        if (!e->active) continue;
        if (e->start) { e->start = 0; e->stopReq = 0; /* hw start applied at next frame */ hw[i].active = -1; }
        else if (!hw[i].active) { chan_finished(i); continue; }
        voices++;
        int vol = tblDecibelSq[e->velocity & 127];
        int pitch = (e->key - e->rootKey) * 64;
        /* envelope */
        switch (e->envStatus) {
        case ENV_ATTACK: e->envAttn = -((-e->envAttn * e->envAttack) >> 8); if (e->envAttn == 0) e->envStatus = ENV_DECAY; break;
        case ENV_DECAY: { int sus = tblDecibelSq[e->envSustain & 127] << 7; e->envAttn -= e->envDecay; if (e->envAttn <= sus) { e->envAttn = sus; e->envStatus = ENV_SUSTAIN; } break; }
        case ENV_SUSTAIN: break;
        case ENV_RELEASE: e->envAttn -= e->envRelease; break;
        }
        vol += e->envAttn >> 7;
        /* sweep */
        if (e->sweepPitch && e->sweepCounter < e->sweepLength) {
            int64_t r = (int64_t)e->sweepPitch * (e->sweepLength - e->sweepCounter);
            pitch += (int)(r / e->sweepLength);
            if (e->autoSweep) e->sweepCounter++;
        }
        vol += e->userDecay; vol += e->userDecay2; pitch += e->userPitch;
        /* lfo */
        int lfo = 0;
        {
            int64_t r = e->lfo.target == 1 ? 60 : 64;
            int val = 0;
            if (e->lfo.depth && e->lfoDelayCnt >= e->lfo.delay) val = sin_idx(e->lfoCounter >> 8) * e->lfo.depth * e->lfo.range;
            lfo = (int)((r * val) >> 14);
            if (e->lfoDelayCnt < e->lfo.delay) e->lfoDelayCnt++;
            else {
                uint32_t tmp = e->lfoCounter; tmp += e->lfo.speed << 6; tmp >>= 8;
                while (tmp >= 0x80) tmp -= 0x80;
                e->lfoCounter += e->lfo.speed << 6; e->lfoCounter &= 0xFF; e->lfoCounter |= tmp << 8;
            }
        }
        int pan = e->initPan;
        switch (e->lfo.target) { case 1: if (vol > -0x8000) vol += lfo; break; case 2: pan += lfo; break; default: pitch += lfo; break; }
        if (e->panRange != 127) pan = (pan * e->panRange + 0x40) >> 7;
        pan += e->userPan;
        if (e->envStatus == ENV_RELEASE && vol <= -723) {
            e->stopReq = 1; chan_finished(i); voices--;
        } else {
            e->volume = calc_volume(vol);
            int tm = calc_timer(e->baseTimer, pitch);
            if (e->type == 1) tm &= 0xFFFC;
            pan += 0x40; if (pan < 0) pan = 0; else if (pan > 127) pan = 127;
            e->regTimer = tm; e->regPan = pan;
        }
    }
    if (voices > maxVoices) maxVoices = voices;
}

/* apply registers computed in the previous frame (SND_UpdateExChannel) */
static void update_hw(void) {
    for (int i = 0; i < 16; i++) {
        ExChan *e = &ch[i]; HwChan *h = &hw[i];
        if (e->stopReq) { h->active = 0; e->stopReq = 0; }
        if (!e->active) { h->active = 0; continue; }
        if (h->active == -1) {
            h->active = 1; h->type = e->type; h->wave = e->wave; h->duty = e->duty; h->pos = 0;
            h->lfsr = 0x7FFF; h->noiseOut = 0;
        }
        h->vol = e->volume & 0xFF; h->shift = SHIFT_TAB[(e->volume >> 8) & 3];
        h->pan = e->regPan; h->timer = e->regTimer ? e->regTimer : 0x10;
    }
}

static int interp = 0, fullOut = 0;
static void mix(float *out, int n) {
    for (int i = 0; i < 16; i++) {
        HwChan *h = &hw[i];
        if (h->active != 1) continue;
        float g = (float)h->vol / 128.0f / (float)(1 << h->shift) / 32768.0f;
        float gl = g * (128 - h->pan) / 128.0f, gr = g * h->pan / 128.0f;
        uint64_t step = ((uint64_t)512 << 32) / (uint64_t)h->timer;
        for (int s = 0; s < n; s++) {
            float v;
            if (h->type == 0) {
                Wave *w = h->wave; uint64_t ip = h->pos >> 32;
                if (ip >= (uint64_t)w->len) {
                    if (w->loop) { uint64_t ll = w->len - w->loopStart; ip = w->loopStart + (ip - w->loopStart) % ll; h->pos = (ip << 32) | (h->pos & 0xFFFFFFFFu); }
                    else { h->active = 0; break; }
                }
                v = w->pcm[ip];
                if (interp) {
                    uint64_t nx = ip + 1; if (nx >= (uint64_t)w->len) nx = w->loop ? (uint64_t)w->loopStart : ip;
                    float fr = (float)(h->pos & 0xFFFFFFFFu) / 4294967296.0f;
                    v = v + (w->pcm[nx] - v) * fr;
                }
            } else if (h->type == 1) {
                int ph = (int)((h->pos >> 32) & 7);
                v = (h->duty == 7) ? -32767.0f : (ph >= 7 - h->duty ? 32767.0f : -32767.0f);
            } else {
                uint64_t before = h->pos >> 32;
                v = h->noiseOut ? 32767.0f : -32767.0f;
                (void)before;
            }
            out[2 * s] += v * gl; out[2 * s + 1] += v * gr;
            uint64_t old = h->pos >> 32;
            h->pos += step;
            if (h->type == 2) {
                uint64_t nw = h->pos >> 32;
                for (uint64_t k = old; k < nw; k++) {
                    int carry = h->lfsr & 1; h->lfsr >>= 1;
                    if (carry) { h->noiseOut = 0; h->lfsr ^= 0x6000; } else h->noiseOut = 1;
                }
            }
        }
    }
}

/* ---------------- WAV output ---------------- */
static void write_wav(const char *path, const int16_t *pcm, long frames, int chans, int rate) {
    FILE *f = fopen(path, "wb"); if (!f) die("cannot write wav");
    uint32_t data = frames * chans * 2;
    uint8_t h[44]; memcpy(h, "RIFF", 4); uint32_t v = 36 + data; memcpy(h + 4, &v, 4); memcpy(h + 8, "WAVEfmt ", 8);
    v = 16; memcpy(h + 16, &v, 4); uint16_t s = 1; memcpy(h + 20, &s, 2); s = chans; memcpy(h + 22, &s, 2);
    v = rate; memcpy(h + 24, &v, 4); v = rate * chans * 2; memcpy(h + 28, &v, 4); s = chans * 2; memcpy(h + 32, &s, 2);
    s = 16; memcpy(h + 34, &s, 2); memcpy(h + 36, "data", 4); memcpy(h + 40, &data, 4);
    fwrite(h, 1, 44, f); fwrite(pcm, 2, frames * chans, f); fclose(f);
}

static long gcdl(long a, long b) { while (b) { long t = a % b; a = b; b = t; } return a; }
static long sample_at_frame(long f) { return (f * FRAME_SAMPLES_X2) / 2; }

/* Seam point: frames after the loop start frame fS where the loop should begin.
   A loop [A, A+Lf) wraps cleanly when the notes still sounding at A in pass 1
   are the same notes (track, key, tick offset) as those sounding at A+Lf in
   pass 2. Before that point pass 1 can differ from later passes because notes
   from before the loop start still ring, or because channel stealing (more than
   16 voices) cut different notes, so a long note from the end of the loop body
   rings into pass 2 but has no counterpart at the start of pass 1. Search the
   first A >= fS (at note start/end boundaries) where both sets match, ignoring
   notes that start or end within 2 frames of the test point (tick->frame
   jitter). Returns A - fS; *converged = 0 if no such point exists. */
static int alive_at(const NoteLog *n, long f, long endNow) {
    long e = n->endFrame < 0 ? endNow : n->endFrame;
    return n->startFrame <= f - 2 && e > f + 2;
}
static int near_edge(const NoteLog *n, long f, long endNow) {
    long e = n->endFrame < 0 ? endNow : n->endFrame;
    return labs(n->startFrame - f) <= 2 || labs(e - f) <= 2;
}
static int seam_ok(long A, long L, long Lf, long endNow) {
    /* every pass-1 note alive at A needs a pass-2 partner alive at A+Lf and vice versa */
    for (long i = 0; i < nNotes; i++) {
        const NoteLog *p = &notes[i];
        if (p->startFrame > A + Lf + 2) break;
        int side;
        if (alive_at(p, A, endNow) && !near_edge(p, A, endNow)) side = 1;
        else if (alive_at(p, A + Lf, endNow) && !near_edge(p, A + Lf, endNow)) side = 2;
        else continue;
        long want = side == 1 ? p->startTick + L : p->startTick - L;
        int found = 0;
        long lo = 0, hi = nNotes; /* notes are logged in non-decreasing tick order */
        while (lo < hi) { long mid = (lo + hi) / 2; if (notes[mid].startTick < want) lo = mid + 1; else hi = mid; }
        for (long j = lo; j < nNotes && notes[j].startTick == want; j++) {
            const NoteLog *q = &notes[j];
            if (q->track != p->track || q->key != p->key) continue;
            long f = side == 1 ? A + Lf : A;
            if (alive_at(q, f, endNow) || near_edge(q, f, endNow)) { found = 1; break; }
        }
        if (!found) return 0;
    }
    return 1;
}
static long loop_convergence(long S, long L, long fS, long fE, long fE2, int *converged) {
    long Lf = fE - fS;
    long last = fS; /* fallback: end of notes started before the loop start */
    for (long k = 0; k < nNotes; k++) if (notes[k].startTick < S) {
        long ef = notes[k].endFrame < 0 ? frameNo : notes[k].endFrame; if (ef > last) last = ef; }
    *converged = 0;
    if (fE2 < 0) { long D = last - fS; if (D > Lf) D = Lf; return D; }
    /* candidate points: fS and every note start/end inside pass 1 (and pass-2 ones shifted back) */
    long nc = 0, *cand = malloc(sizeof(long) * (4 * nNotes + 2));
    cand[nc++] = fS;
    for (long k = 0; k < nNotes; k++) {
        long st = notes[k].startFrame, e = notes[k].endFrame < 0 ? frameNo : notes[k].endFrame;
        long pts[4] = {st + 3, e + 3, st - Lf + 3, e - Lf + 3};
        for (int m = 0; m < 4; m++) if (pts[m] > fS && pts[m] < fE) cand[nc++] = pts[m];
    }
    /* sort candidates (insertion into a simple counting order) */
    for (long i = 1; i < nc; i++) { long v = cand[i], j = i - 1; while (j >= 0 && cand[j] > v) { cand[j + 1] = cand[j]; j--; } cand[j + 1] = v; }
    long D = -1;
    for (long i = 0; i < nc; i++) {
        if (i && cand[i] == cand[i - 1]) continue;
        if (seam_ok(cand[i], L, Lf, frameNo)) { D = cand[i] - fS; *converged = 1; break; }
    }
    free(cand);
    if (D < 0) { D = last - fS; if (D > Lf) D = Lf; }
    return D;
}

/* ---------------- render ---------------- */
static int cmd_render(int argc, char **argv) {
    const char *sdatP = 0, *tabP = 0, *outP = 0; int seqId = -1, bankId = -1, sw[4] = {-1, -1, -1, -1}, vol = 127;
    long start = 0; double maxSec = 600, holdSec = 0; int ppr = 64, gainTl = 0; int held = 0, extraLoops = 0;
    for (int i = 2; i < argc; i++) {
        const char *a = argv[i], *v = i + 1 < argc ? argv[i + 1] : "";
        if (!strcmp(a, "--sdat")) sdatP = v, i++;
        else if (!strcmp(a, "--tables")) tabP = v, i++;
        else if (!strcmp(a, "--out")) outP = v, i++;
        else if (!strcmp(a, "--seq")) seqId = atoi(v), i++;
        else if (!strcmp(a, "--start")) start = strtol(v, 0, 0), i++;
        else if (!strcmp(a, "--bank")) bankId = atoi(v), i++;
        else if (!strcmp(a, "--swar")) { sscanf(v, "%d,%d,%d,%d", &sw[0], &sw[1], &sw[2], &sw[3]); i++; }
        else if (!strcmp(a, "--vol")) vol = atoi(v), i++;
        else if (!strcmp(a, "--ppr")) ppr = atoi(v), i++;
        else if (!strcmp(a, "--mask")) { chanMaskAllowed = strtol(v, 0, 0); if (!chanMaskAllowed) chanMaskAllowed = 0xFFFF; i++; }
        else if (!strcmp(a, "--max-sec")) maxSec = atof(v), i++;
        else if (!strcmp(a, "--interp")) interp = 1;
        else if (!strcmp(a, "--full")) fullOut = 1;
        else if (!strcmp(a, "--hold-sec")) holdSec = atof(v), i++;
        else if (!strcmp(a, "--gain-tl")) gainTl = atoi(v), i++;
        else if (!strcmp(a, "--extra-loops")) extraLoops = atoi(v), i++; /* validation: keep rendering N more loop passes (with --full) */
        else die("bad arg");
    }
    if (!sdatP || !tabP || !outP || seqId < 0) die("missing args");
    load_sdat(sdatP); load_tables(tabP);
    uint32_t fsz; const uint8_t *sf = sdat_file(seqId, &fsz);
    if (!sf) die("bad seq file");
    uint32_t dataOff;
    if (!memcmp(sf, "SSEQ", 4) || !memcmp(sf, "SSAR", 4)) dataOff = rd32(sf + 0x18); else die("not SSEQ/SSAR");
    seqData = sf + dataOff; seqLen = fsz - dataOff;
    if (bankId >= 0) { bank = sdat_file(bankId, &bankSize); if (bank && memcmp(bank, "SBNK", 4)) bank = NULL; }
    for (int i = 0; i < 4; i++) load_swar(i, sw[i]);
    playerPrio = ppr;
    /* NNS player start (InitPlayer + StartSeq/StartSeqArc + Sound_PlaySeq*):
       fader 127<<8, initial volume = SEQ/SSAR info volume, SetVolume(127) */
    memset(&nns, 0, sizeof nns);
    nns.fader.origin = nns.fader.target = 127 << 8; nns.fader.frame = 1;
    nns.status = 1; nns.volume1 = vol; nns.volume2 = 127; nns.playerVolume = 127;
    nns.volume = 0; playerFader = 0;
    long nnsAcc = 1919555; /* NNS main at driver frame 0, then every 191.9555/60 driver frames */
    for (int i = 0; i < 32; i++) vars[i] = -1;
    for (int i = 0; i < 16; i++) { tracks[i].firstTick = malloc(sizeof(int) * (seqLen + 1)); tracks[i].snap = calloc(seqLen + 1, sizeof(int32_t) * SNAP_N); for (uint32_t k = 0; k <= seqLen; k++) tracks[i].firstTick[k] = -1; track_init(&tracks[i]); ch[i].nextInTrack = -1; ch[i].logIdx = -1; }
    /* player start */
    trackMask = 1; uint32_t pc = start;
    if (pc + 3 <= seqLen && seqData[pc] == 0xFE) { trackMask = rd16(seqData + pc + 1) | 1; pc += 3; }
    track_start(&tracks[0], pc);

    long maxFrames = (long)(maxSec * 191.9555) + 1;
    long cap = sample_at_frame(maxFrames + 2) + 8;
    float *mixbuf = calloc((size_t)cap * 2, sizeof(float));
    long *tickFrame = calloc(maxFrames * 8 + 1024, sizeof(long)); long tickCap = maxFrames * 8 + 1024;
    long loopS = -1, loopL = -1, fS = -1, fE = -1, fE2 = -1, stopFrame = -1, D = 0; int converged = 1;
    int ended = 0, loopKnown = 0, lcmCapped = 0, firstPassDiffers = 0;
    long silentRun = 0;
    for (frameNo = 0; frameNo < maxFrames; frameNo++) {
        if (nnsAcc >= 1919555) { nnsAcc -= 1919555; if (nns.status) nns_player_main(); }
        nnsAcc += 600000;
        update_hw();
        long t0 = tickNo;
        seq_main();
        for (long k = t0; k < tickNo && k < tickCap; k++) tickFrame[k] = frameNo;
        exchannel_main();
        long s0 = sample_at_frame(frameNo), s1 = sample_at_frame(frameNo + 1);
        mix(mixbuf + 2 * s0, (int)(s1 - s0));
        /* loop detection */
        if (!loopKnown && playerActive) {
            int all = 1, any = 0; long S = 0, L = 1;
            for (int i = 0; i < 16; i++) {
                Track *t = &tracks[i];
                if (!t->alloc) continue;
                if (t->active && !t->looped) { all = 0; break; }
                if (t->looped) {
                    long p = t->loopEndTick - t->loopStartTick;
                    if (p <= 0) continue;
                    any = 1; if (t->loopStartTick > S) S = t->loopStartTick;
                    long g = gcdl(L, p); if (L / g * p > 200000) { lcmCapped = 1; if (p > L) L = p; } else L = L / g * p;
                }
            }
            if (all && any) {
                loopKnown = 1; loopS = S; loopL = L;
                /* If some track's parameters at its loop jump differ from those it had
                   when it first reached the loop target, the first pass through the
                   loop body sounds different from later passes (the DS plays it that
                   way too). Then loop on the second pass: start = S + L. */
                for (int i = 0; i < 16; i++) if (tracks[i].alloc && tracks[i].looped && tracks[i].stateDiffers) firstPassDiffers = 1;
                if (firstPassDiffers) loopS = S + L;
            }
        }
        /* Render two full loop passes (plus 2 s) so the pass-1 / pass-2 note
           lifetimes can be compared (see loop_convergence). */
        if (loopKnown && fS < 0 && tickNo > loopS + 2 * loopL) {
            fS = tickFrame[loopS]; fE = tickFrame[loopS + loopL]; fE2 = tickFrame[loopS + 2 * loopL];
            stopFrame = fE2 + 2 * 192 + 1 + extraLoops * (fE - fS);
        }
        if (stopFrame >= 0 && frameNo >= stopFrame) { frameNo++; break; }
        /* --hold-sec: a sequence whose remaining tracks all wait on a held note
           (length-0 note on a looping wave; the game stops these itself) and that
           has not looped by then is stopped like NNS_SndPlayerStopSeq(handle,
           HOLD_FADE_NNS_FRAMES) (NNSi_SndPlayerStopSeq: fader -> 0, status 2,
           ForceStopSeq when the fade ends): the per-call fade length the game
           uses is not known here. */
        int stuck = 1;
        for (int i = 0; i < 16; i++) if (tracks[i].active && !tracks[i].noteFinishWait && !tracks[i].looped) stuck = 0;
        if (holdSec > 0 && playerActive && !loopKnown && stuck && !held && frameNo >= (long)(holdSec * 191.9555)) {
            held = 1;
            nns_fader_set(&nns.fader, 0, HOLD_FADE_NNS_FRAMES); nns.status = 2;
        }
        if (!playerActive) {
            int any = 0; for (int i = 0; i < 16; i++) if (ch[i].active || hw[i].active) any = 1;
            if (!any) { if (++silentRun > 2) { ended = 1; frameNo++; break; } }
        }
    }
    long totalFrames = frameNo;
    if (loopKnown && fS < 0 && loopS + loopL < tickNo) { /* hit max-sec before pass 2 ended */
        fS = tickFrame[loopS]; fE = tickFrame[loopS + loopL]; fE2 = -1;
    }
    if (fS >= 0) D = loop_convergence(loopS, loopL, fS, fE, fE2, &converged);
    if (getenv("SSEQ_DEBUG")) {
        for (int i = 0; i < 16; i++) if (tracks[i].alloc)
            fprintf(stderr, "track %d looped=%d start=%ld end=%ld active=%d\n", i, tracks[i].looped, tracks[i].loopStartTick, tracks[i].loopEndTick, tracks[i].active);
        fprintf(stderr, "fS=%ld fE=%ld D=%ld tempo=%d\n", fS, fE, D, tempo);
    }
    long nSamp = sample_at_frame(totalFrames);
    long lsSamp = -1, leSamp = -1;
    if (fS >= 0) { lsSamp = sample_at_frame(fS + D + 1); leSamp = sample_at_frame(fE + D + 1); }
    /* seam check: RMS difference between audio right after loop start and right after loop end */
    double seamErr = -1, seamRef = 0;
    if (lsSamp >= 0) {
        long n = nSamp - leSamp; if (n > 32728) n = 32728;
        double se = 0, sr = 0;
        for (long k = 0; k < n * 2; k++) { double a = mixbuf[2 * lsSamp + k], b = mixbuf[2 * leSamp + k]; se += (a - b) * (a - b); sr += a * a; }
        seamErr = n ? sqrt(se / (2.0 * n)) : -1; seamRef = n ? sqrt(sr / (2.0 * n)) : 0;
    }
    /* Loop seam: tick->frame quantisation drifts between loop passes when the
       loop length in frames is not an integer (tempo/240 ticks per frame), so the
       audio at loop end and at loop start differ by <=1 frame of note timing.
       Crossfade the last XF samples before loop end into the audio that precedes
       loop start, so the wrap from loop_end to loop_start is continuous. */
    if (lsSamp >= 0 && !fullOut) {
        /* 2048-sample crossfade whose last LOOP_TAIL_EXACT samples are exactly the
           audio before loop start (weight 1): the ADPCM encoder (aicaenc) joins
           its state trajectory there, see adpcm_encode_channel */
        long xf = 2048, ramp; if (xf > lsSamp) xf = lsSamp;
        ramp = xf > LOOP_TAIL_EXACT ? xf - LOOP_TAIL_EXACT : xf;
        for (long k = 0; k < xf; k++) {
            float w = k >= ramp ? 1.0f : (float)(k + 1) / (float)(ramp + 1);
            for (int c = 0; c < 2; c++) {
                long a = 2 * (leSamp - xf + k) + c, b = 2 * (lsSamp - xf + k) + c;
                mixbuf[a] = mixbuf[a] * (1 - w) + mixbuf[b] * w;
            }
        }
    }
    long outLen = (leSamp >= 0 && !fullOut) ? leSamp : nSamp;
    int16_t *pcm = malloc(sizeof(int16_t) * 2 * (outLen + 1));
    long clips = 0; double peak = 0, sum2 = 0, dsPeak = 0;
    const double gain = pow(2.0, -gainTl / 16.0);
    for (long k = 0; k < outLen * 2; k++) {
        double v = mixbuf[k] * (127.0 / 128.0) * 32768.0; /* DS mixer output, master volume 127 */
        if (fabs(v) > dsPeak) dsPeak = fabs(v);
        v *= gain;                                          /* port mix gain */
        if (fabs(v) > peak) peak = fabs(v);
        sum2 += v * v;
        if (v > 32767) { v = 32767; clips++; } else if (v < -32768) { v = -32768; clips++; }
        pcm[k] = (int16_t)lrint(v);
    }
    /* trim trailing silence for one-shots */
    if (lsSamp < 0) { while (outLen > 1 && pcm[2 * outLen - 1] == 0 && pcm[2 * outLen - 2] == 0) outLen--; }
    write_wav(outP, pcm, outLen, 2, OUT_RATE);
    int stereoDiff = 0;
    for (long k = 0; k < outLen; k++) if (abs(pcm[2 * k] - pcm[2 * k + 1]) > 2) { stereoDiff = 1; break; }
    printf("{\"rate\":%d,\"samples\":%ld,\"frames\":%ld,\"ticks\":%ld,\"ended\":%s,\"loop\":%s,"
           "\"loop_start\":%ld,\"loop_end\":%ld,\"loop_start_tick\":%ld,\"loop_len_ticks\":%ld,\"tail_frames\":%ld,"
           "\"lcm_capped\":%s,\"seam_wave_rms\":%.3f,\"seam_ref_rms\":%.3f,\"gain_tl\":%d,\"ds_peak\":%.1f,\"peak\":%.1f,\"rms\":%.1f,\"clipped\":%ld,"
           "\"max_voices\":%d,\"steals\":%d,\"notes\":%ld,\"stereo\":%s,\"hit_max\":%s,\"held_stopped\":%s,\"inst_missing\":%d,\"wave_missing\":%d,\"alloc_fail\":%d,\"first_pass_differs\":%s,\"converged\":%s}\n",
           OUT_RATE, outLen, totalFrames, tickNo, ended ? "true" : "false", lsSamp >= 0 ? "true" : "false",
           lsSamp, leSamp, loopS, loopL, D, lcmCapped ? "true" : "false", seamErr * 32768 * gain, seamRef * 32768 * gain, gainTl, dsPeak, peak,
           outLen ? sqrt(sum2 / (2.0 * outLen)) : 0.0, clips, maxVoices, stealCount, nNotes,
           stereoDiff ? "true" : "false", (!ended && lsSamp < 0) ? "true" : "false", held ? "true" : "false", instMissing, waveMissing, allocFail, firstPassDiffers ? "true" : "false", converged ? "true" : "false");
    return 0;
}

static int cmd_swar(int argc, char **argv) {
    const char *sdatP = 0, *outd = 0; int fid = -1;
    for (int i = 2; i < argc; i++) {
        if (!strcmp(argv[i], "--sdat")) sdatP = argv[++i];
        else if (!strcmp(argv[i], "--file")) fid = atoi(argv[++i]);
        else if (!strcmp(argv[i], "--outdir")) outd = argv[++i];
    }
    if (!sdatP || !outd || fid < 0) die("missing args");
    load_sdat(sdatP); load_swar(0, fid);
    if (!swars[0].data) die("not a SWAR");
    printf("[");
    for (int i = 0; i < swars[0].count; i++) {
        Wave *w = get_wave(0, i);
        if (i) printf(",");
        if (!w) { printf("null"); continue; }
        char p[1024]; snprintf(p, sizeof p, "%s/%04d.wav", outd, i);
        int rate = (int)lrint(16756991.0 / w->timer);
        write_wav(p, w->pcm, w->len, 1, rate);
        printf("{\"index\":%d,\"format\":%d,\"rate_field\":%d,\"timer\":%d,\"rate\":%d,\"samples\":%d,\"loop\":%s,\"loop_start\":%d}",
               i, w->fmt, w->rate, w->timer, rate, w->len, w->loop ? "true" : "false", w->loopStart);
    }
    printf("]\n");
    return 0;
}

/* STRM: HEAD (type, loop, channels, rate, loop start, samples, data offset,
   blocks, block length, samples/block, last block length/samples), then DATA
   with blocks interleaved per channel; each IMA-ADPCM block has its own header. */
static void ima_decode_block(const uint8_t *d, uint32_t len, int16_t *out, uint32_t maxSamp, int stride) {
    if (len < 4) return;
    int s = (int16_t)rd16(d), idx = d[2] & 0x7F; if (idx > 88) idx = 88;
    uint32_t k = 0;
    for (uint32_t i = 4; i < len && k < maxSamp; i++) {
        for (int h = 0; h < 2 && k < maxSamp; h++) {
            int nib = h ? d[i] >> 4 : d[i] & 15, st = IMA_STEP[idx], diff = st >> 3;
            if (nib & 1) diff += st >> 2;
            if (nib & 2) diff += st >> 1;
            if (nib & 4) diff += st;
            if (nib & 8) { s -= diff; if (s < -0x7FFF) s = -0x7FFF; } else { s += diff; if (s > 0x7FFF) s = 0x7FFF; }
            idx += IMA_INDEX[nib & 7]; if (idx < 0) idx = 0; if (idx > 88) idx = 88;
            out[(k++) * stride] = (int16_t)s;
        }
    }
}
static int cmd_strm(int argc, char **argv) {
    const char *sdatP = 0, *outP = 0; int fid = -1;
    for (int i = 2; i < argc; i++) {
        if (!strcmp(argv[i], "--sdat")) sdatP = argv[++i];
        else if (!strcmp(argv[i], "--file")) fid = atoi(argv[++i]);
        else if (!strcmp(argv[i], "--out")) outP = argv[++i];
    }
    if (!sdatP || !outP || fid < 0) die("missing args");
    load_sdat(sdatP);
    uint32_t sz; const uint8_t *f = sdat_file(fid, &sz);
    if (!f || memcmp(f, "STRM", 4) || memcmp(f + 0x10, "HEAD", 4)) die("not a STRM");
    const uint8_t *h = f + 0x10;
    int type = h[8], loop = h[9], chans = h[10], rate = rd16(h + 12);
    uint32_t loopStart = rd32(h + 16), nsamp = rd32(h + 20), dataOff = rd32(h + 24), nblocks = rd32(h + 28);
    uint32_t blen = rd32(h + 32), bsamp = rd32(h + 36), lastLen = rd32(h + 40), lastSamp = rd32(h + 44);
    if (chans < 1 || chans > 2) die("bad channel count");
    int16_t *out = calloc((size_t)nblocks * bsamp * chans + 16, 2);
    const uint8_t *p = f + dataOff;
    uint32_t pos = 0;
    for (uint32_t b = 0; b < nblocks; b++) {
        uint32_t L = b == nblocks - 1 ? lastLen : blen, ns = b == nblocks - 1 ? lastSamp : bsamp;
        for (int c = 0; c < chans; c++) {
            if (p + L > f + sz) die("STRM truncated");
            int16_t *o = out + (size_t)pos * chans + c;
            if (type == 2) ima_decode_block(p, L, o, ns, chans);
            else if (type == 1) for (uint32_t k = 0; k < ns && 2 * k + 1 < L; k++) o[k * chans] = (int16_t)rd16(p + 2 * k);
            else for (uint32_t k = 0; k < ns && k < L; k++) o[k * chans] = (int16_t)((int8_t)p[k] * 256);
            p += L;
        }
        pos += ns;
    }
    if (pos > nsamp) pos = nsamp;
    write_wav(outP, out, pos, chans, rate);
    printf("{\"type\":%d,\"rate\":%d,\"channels\":%d,\"samples\":%u,\"loop\":%s,\"loop_start\":%u}\n",
           type, rate, chans, pos, loop ? "true" : "false", loopStart);
    return 0;
}

/* AICA / Yamaha 4-bit ADPCM decoder (same step model as KOS utils/wav2adpcm) */
static void aica_decode(const uint8_t *src, long nsamp, int stride, int16_t *dst, int dstStride) {
    static const int stepTab[8] = {230, 230, 230, 230, 307, 409, 512, 614};
    int hist = 0, step = 127;
    for (long i = 0; i < nsamp; i++) {
        int byte = src[(i / 2) * stride];
        int nib = (i & 1) ? (byte >> 4) & 15 : byte & 15;
        int d = nib & 7, diff = ((1 + (d << 1)) * step) >> 3;
        if (diff > 32767) diff = 32767;
        hist = (nib & 8) ? hist - diff : hist + diff;
        if (hist > 32767) hist = 32767; else if (hist < -32768) hist = -32768;
        step = (stepTab[d] * step) >> 8; if (step < 127) step = 127; else if (step > 24576) step = 24576;
        dst[i * dstStride] = (int16_t)hist;
    }
}
static const int ENC_STEP[8] = {230, 230, 230, 230, 307, 409, 512, 614};
/* aicadec ... --loop LS,LE --passes N (stereo fmt 0x20 only): decode like the
   snd_stream long-stream player: intro, then N passes of [LS, LE) without
   resetting the decoder; prints the per-pass state at LE and, per pass, the
   max / mean difference to the first pass (loop-seam drift check) */
static int aicadec_loop(const uint8_t *data, long ns, long ls, long le, int passes, const char *outp, uint32_t rate) {
    long L = le - ls, total = ls + L * passes;
    int16_t *out = calloc(total * 2, 2);
    int hist[2] = {0, 0}, step[2] = {127, 127};
    (void)ns;
    printf("{\"passes\":[");
    for (int ps = 0; ps <= passes; ps++) {
        long a = ps == 0 ? 0 : ls, b = ps == 0 ? ls : le, o = ps == 0 ? 0 : ls + L * (ps - 1);
        for (long i = a; i < b; i++) for (int c = 0; c < 2; c++) {
            int nib = c == 0 ? (data[i] >> 4) & 15 : data[i] & 15;
            int dd = nib & 7, diff = ((1 + (dd << 1)) * step[c]) >> 3;
            hist[c] = (nib & 8) ? hist[c] - diff : hist[c] + diff;
            if (hist[c] > 32767) hist[c] = 32767; else if (hist[c] < -32768) hist[c] = -32768;
            step[c] = (ENC_STEP[dd] * step[c]) >> 8; if (step[c] < 127) step[c] = 127; else if (step[c] > 24576) step[c] = 24576;
            out[2 * (o + i - a) + c] = (int16_t)hist[c];
        }
        if (ps >= 1) {
            double mx = 0, mean[2] = {0, 0};
            for (long i = 0; i < L; i++) for (int c = 0; c < 2; c++) {
                double dlt = out[2 * (ls + L * (ps - 1) + i) + c] - out[2 * (ls + i) + c];
                if (fabs(dlt) > mx) mx = fabs(dlt);
                mean[c] += dlt / L;
            }
            printf("%s{\"pass\":%d,\"end_state\":[[%d,%d],[%d,%d]],\"max_diff_vs_pass1\":%.0f,\"mean_diff_vs_pass1\":[%.1f,%.1f]}",
                   ps > 1 ? "," : "", ps, hist[0], step[0], hist[1], step[1], mx, mean[0], mean[1]);
        }
    }
    printf("]}\n");
    write_wav(outp, out, total, 2, rate);
    return 0;
}
static int cmd_aicadec(int argc, char **argv) {
    if (argc < 4) die("aicadec IN OUT");
    long lls = -1, lle = -1; int passes = 0;
    for (int i = 4; i < argc; i++) {
        if (!strcmp(argv[i], "--loop") && i + 1 < argc) sscanf(argv[++i], "%ld,%ld", &lls, &lle);
        else if (!strcmp(argv[i], "--passes") && i + 1 < argc) passes = atoi(argv[++i]);
    }
    FILE *f = fopen(argv[2], "rb"); if (!f) die("open in");
    fseek(f, 0, SEEK_END); long n = ftell(f); fseek(f, 0, SEEK_SET);
    uint8_t *d = malloc(n); if (fread(d, 1, n, f) != (size_t)n) die("read"); fclose(f);
    int fmt = rd16(d + 20), chans = rd16(d + 22); uint32_t rate = rd32(d + 24);
    long p = 12; uint32_t dsz = 0; const uint8_t *data = NULL;
    while (p + 8 <= n) { uint32_t cs = rd32(d + p + 4); if (!memcmp(d + p, "data", 4)) { data = d + p + 8; dsz = cs; break; } p += 8 + cs + (cs & 1); }
    if (!data) die("no data chunk");
    long ns = (chans == 2) ? dsz : dsz * 2; /* samples per channel */
    if (passes > 0) { if (chans != 2 || fmt != 0x20) die("--passes: stereo fmt 0x20 only"); return aicadec_loop(data, ns, lls, lle, passes, argv[3], rate); }
    int16_t *out = calloc(ns * chans, 2);
    if (chans == 1) aica_decode(data, ns, 1, out, 1);
    else if (fmt == 0x14) { aica_decode(data, ns, 1, out, 2); aica_decode(data + dsz / 2, ns, 1, out + 1, 2); }
    else {
        /* fmt 0x20: byte i = L nibble<<4 | R nibble, one sample of each channel per byte */
        static const int stepTab[8] = {230, 230, 230, 230, 307, 409, 512, 614};
        for (int c = 0; c < 2; c++) {
            int hist = 0, step = 127;
            for (long i = 0; i < ns; i++) {
                int nib = c == 0 ? (data[i] >> 4) & 15 : data[i] & 15;
                int dd = nib & 7, diff = ((1 + (dd << 1)) * step) >> 3;
                hist = (nib & 8) ? hist - diff : hist + diff;
                if (hist > 32767) hist = 32767; else if (hist < -32768) hist = -32768;
                step = (stepTab[dd] * step) >> 8; if (step < 127) step = 127; else if (step > 24576) step = 24576;
                out[2 * i + c] = (int16_t)hist;
            }
        }
    }
    write_wav(argv[3], out, ns, chans, rate);
    return 0;
}

/* ---------------- AICA ADPCM encoder with a loop-consistent state ----------------
 * Same quantiser as KOS utils/wav2adpcm (pcm2adpcm: (x & -8) - history, code =
 * |d| / step, Yamaha step update; byte layout of `wav2adpcm -t` (fmt 0x14 mono,
 * low nibble first) and `-i` (fmt 0x20 stereo: byte = L nibble << 4 | R nibble)).
 * The AICA "long stream" decoder (snd_stream) and Flycast keep the decoder state
 * (previous sample, step) across the ring buffer and never reset it, so when the
 * player jumps from loop_end back to loop_start the decoder enters the loop body
 * with the state reached at loop_end; a plain encoder produced the loop body for
 * the state reached at loop_start after the intro, so every pass added the
 * difference as a lasting DC step (z1: 55 / 26 LSB per pass).
 * Here the tail of the loop body is joined onto the trajectory that leads into
 * loop_start (see adpcm_encode_channel), so the decoder state at loop_end equals
 * the state at loop_start exactly and every pass decodes like the first. */
typedef struct { int hist, step; } AdpcmState;
static inline void adpcm_apply(AdpcmState *s, int code) {
    int d = code & 7, diff = ((1 + (d << 1)) * s->step) >> 3;
    if (diff > 32767) diff = 32767;
    int v = (code & 8) ? s->hist - diff : s->hist + diff;
    s->hist = v > 32767 ? 32767 : v < -32768 ? -32768 : v;
    int ns = (ENC_STEP[d] * s->step) >> 8;
    s->step = ns < 127 ? 127 : ns > 24576 ? 24576 : ns;
}
static inline int adpcm_code(const AdpcmState *s, int x) {
    int d = (x & -8) - s->hist;
    unsigned c = ((unsigned)abs(d) << 16) / ((unsigned)s->step << 14);
    if (c > 7) c = 7;
    return (int)c | (d < 0 ? 8 : 0);
}
static AdpcmState adpcm_encode(AdpcmState s, const int16_t *x, long stride, long n, uint8_t *codes) {
    for (long i = 0; i < n; i++) { int c = adpcm_code(&s, x[i * stride]); codes[i] = (uint8_t)c; adpcm_apply(&s, c); }
    return s;
}
/* one channel: codes[0..n), loop [ls, le) (ls < 0: no loop).
   Loop join: the last JOIN_M samples before le are set equal to the JOIN_M
   samples before ls (the renderer already crossfades this region towards
   them), the file is encoded normally (pass 1 and the intro are untouched),
   then inside that tail the encoder trajectory B (towards le) is steered with
   k replaced codes onto trajectory A (towards ls) at some sample; from there B
   uses A's codes, so the state at le equals the state at ls exactly and every
   loop pass decodes like the first. x is modified in place (tail copy). */
#define JOIN_M LOOP_TAIL_EXACT
static void adpcm_encode_channel(int16_t *x, long stride, long n, long ls, long le, uint8_t *codes, char *rep, size_t repLen) {
    AdpcmState s0 = {0, 127};
    if (ls < 0 || ls >= le || le > n) { adpcm_encode(s0, x, stride, n, codes); snprintf(rep, repLen, "\"noloop\""); return; }
    long M = JOIN_M;
    if (M > ls) M = ls;
    if (M > le - ls) M = le - ls;
    long changed = 0;
    for (long i = 0; i < M; i++) {
        int16_t v = x[(ls - M + i) * stride];
        if (x[(le - M + i) * stride] != v) changed++;
        x[(le - M + i) * stride] = v;
    }
    adpcm_encode(s0, x, stride, n, codes);
    /* states after each sample of A = [ls-M, ls) and B = [le-M, le) */
    AdpcmState *A = malloc(sizeof(AdpcmState) * (M + 1)), *B = malloc(sizeof(AdpcmState) * (M + 1));
    AdpcmState s = s0;
    for (long i = 0; i < ls - M; i++) adpcm_apply(&s, codes[i]);
    A[0] = s; for (long i = 0; i < M; i++) { adpcm_apply(&s, codes[ls - M + i]); A[i + 1] = s; }
    s = s0;
    for (long i = 0; i < le - M; i++) adpcm_apply(&s, codes[i]);
    B[0] = s; for (long i = 0; i < M; i++) { adpcm_apply(&s, codes[le - M + i]); B[i + 1] = s; }
    /* join B onto A: first sample where B's natural state already equals A's;
       else a meet-in-the-middle search: at tail sample jj replace K = KF + KB
       codes so that B lands exactly on A's state K samples later (KF codes
       forward from B, KB codes backward from A's state through the inverse step
       update, matched in a hash table); the hit with the smallest sample error
       over the tail wins. From the join on, B uses A's codes. */
    long joinAt = -1, steered = 0; double steerErr = 0;
    for (long jj = 0; jj <= M; jj++) if (B[jj].hist == A[jj].hist && B[jj].step == A[jj].step) { joinAt = jj; break; }
    for (int attempt = 0; attempt < 2 && joinAt < 0; attempt++) {
    if (attempt == 1) {
        /* step chase: the step only moves x0.9 .. x2.4 per code, so B first
           drives its step towards A's (code magnitude chosen for the step, sign
           and magnitude among those for the signal); once the steps are within
           ~10 % it encodes normally. B[] / codes then hold this trajectory. */
        AdpcmState b = B[0];
        for (long jj = 0; jj < M; jj++) {
            int c = adpcm_code(&b, x[(le - M + jj) * stride]);
            double ratio = log((double)b.step / A[jj].step);
            if (fabs(ratio) > 0.1) {
                int best = c; double bestScore = 1e300;
                for (int cc = 0; cc < 16; cc++) {
                    AdpcmState t = b; adpcm_apply(&t, cc);
                    double sr = fabs(log((double)t.step / A[jj + 1].step));
                    double he = fabs((double)t.hist - x[(le - M + jj) * stride]) / A[jj + 1].step;
                    double score = sr * 8 + he;
                    if (score < bestScore) { bestScore = score; best = cc; }
                }
                if (best != c) { steered++; }
                c = best;
            }
            codes[le - M + jj] = (uint8_t)c; adpcm_apply(&b, c); B[jj + 1] = b;
            if (b.hist == A[jj + 1].hist && b.step == A[jj + 1].step) { joinAt = jj + 1; break; }
        }
    }
    if (joinAt < 0) {
        static const int KF_TAB[3] = {2, 3, 4}, KB_TAB[3] = {2, 3, 4};
        const long HS = 1L << 18;
        int32_t *hk = malloc(sizeof(int32_t) * HS * 2); uint16_t *hc = malloc(sizeof(uint16_t) * HS);
        uint32_t *ep = calloc(HS, sizeof(uint32_t)), epoch = 0;
        double bestE = 1e300; long bestJ = -1; int bestKF = 0, bestKB = 0; long bestF = 0, bestB = 0;
        for (int pass = 0; pass < 3 && bestJ < 0; pass++) {
            int KF = KF_TAB[pass], KB = KB_TAB[pass], K = KF + KB;
            for (long jj = 0; jj + K <= M; jj += 1) {
                /* backward set from A[jj+K] */
                epoch++;
                long nb = 1L << (4 * KB);
                for (long m = 0; m < nb; m++) {
                    /* walk back KB codes: code q applies at sample jj+K-1-q */
                    int hist = A[jj + K].hist, step = A[jj + K].step, okb = 1; long mm = m;
                    for (int q = 0; q < KB && okb; q++) {
                        int c = (int)(mm & 15); mm >>= 4;
                        int d = c & 7, sp = -1;
                        /* predecessor step: clamp(floor(ENC_STEP[d]*sp/256)) == step; take the smallest */
                        long lo = ((long)step * 256 + ENC_STEP[d] - 1) / ENC_STEP[d];
                        if (lo < 127) lo = 127;
                        for (long t = lo; t <= 24576 && t < lo + 4; t++) {
                            long ns2 = (ENC_STEP[d] * t) >> 8; if (ns2 < 127) ns2 = 127; if (ns2 > 24576) ns2 = 24576;
                            if (ns2 == step) { sp = (int)t; break; }
                        }
                        if (sp < 0) { okb = 0; break; }
                        int diff = ((1 + (d << 1)) * sp) >> 3; if (diff > 32767) diff = 32767;
                        int hp = (c & 8) ? hist + diff : hist - diff;
                        if (hp > 32767 || hp < -32768) { okb = 0; break; }
                        hist = hp; step = sp;
                    }
                    if (!okb) continue;
                    unsigned long key = ((unsigned long)(uint32_t)hist * 2654435761u ^ (unsigned long)step * 40503u) & (HS - 1);
                    while (ep[key] == epoch && !(hk[2 * key] == hist && hk[2 * key + 1] == step)) key = (key + 1) & (HS - 1);
                    ep[key] = epoch; hk[2 * key] = hist; hk[2 * key + 1] = step; hc[key] = (uint16_t)m;
                }
                long nf = 1L << (4 * KF);
                for (long m = 0; m < nf; m++) {
                    AdpcmState t = B[jj]; long mm = m; double err = 0;
                    for (int q = 0; q < KF; q++) { adpcm_apply(&t, (int)(mm & 15)); mm >>= 4; double ee = t.hist - x[(le - M + jj + q) * stride]; err += ee * ee; }
                    unsigned long key = ((unsigned long)(uint32_t)t.hist * 2654435761u ^ (unsigned long)t.step * 40503u) & (HS - 1);
                    while (ep[key] == epoch && !(hk[2 * key] == t.hist && hk[2 * key + 1] == t.step)) key = (key + 1) & (HS - 1);
                    if (ep[key] != epoch) continue;
                    /* forward-check the backward half (exact) and add its error */
                    AdpcmState u = t; long mb = hc[key]; int cb[8];
                    for (int q = 0; q < KB; q++) { cb[KB - 1 - q] = (int)(mb & 15); mb >>= 4; }
                    for (int q = 0; q < KB; q++) { adpcm_apply(&u, cb[q]); double ee = u.hist - x[(le - M + jj + KF + q) * stride]; err += ee * ee; }
                    if (u.hist != A[jj + K].hist || u.step != A[jj + K].step) continue;
                    if (err < bestE) { bestE = err; bestJ = jj; bestKF = KF; bestKB = KB; bestF = m; bestB = hc[key]; }
                }
            }
        }
        if (bestJ >= 0) {
            long mm = bestF;
            for (int q = 0; q < bestKF; q++) { codes[le - M + bestJ + q] = (uint8_t)(mm & 15); mm >>= 4; }
            mm = bestB;
            for (int q = 0; q < bestKB; q++) { codes[le - M + bestJ + bestKF + bestKB - 1 - q] = (uint8_t)(mm & 15); mm >>= 4; }
            joinAt = bestJ + bestKF + bestKB; steered += bestKF + bestKB; steerErr = bestE;
        }
        free(hk); free(hc); free(ep);
    }
    }
    if (joinAt >= 0) for (long ii = joinAt; ii < M; ii++) codes[le - M + ii] = codes[ls - M + ii];
    /* verify: state at le after the loop body entered with the state at ls */
    AdpcmState sl = A[M], e = sl;
    for (long i = ls; i < le; i++) adpcm_apply(&e, codes[i]);
    snprintf(rep, repLen, "{\"joined\":%s,\"join_sample\":%ld,\"steer_codes\":%ld,\"steer_rms\":%.0f,\"tail_samples_changed\":%ld,\"seam_mismatch\":[%d,%d]}",
             (e.hist == sl.hist && e.step == sl.step) ? "true" : "false", joinAt, steered,
             steered ? sqrt(steerErr / steered) : 0.0, changed, e.hist - sl.hist, e.step - sl.step);
    free(A); free(B);
}
static void write_wav_fmt(const char *path, const uint8_t *data, uint32_t bytes, int chans, int rate, int fmt) {
    FILE *f = fopen(path, "wb"); if (!f) die("cannot write wav");
    uint8_t h[44]; memcpy(h, "RIFF", 4); uint32_t v = 36 + bytes; memcpy(h + 4, &v, 4); memcpy(h + 8, "WAVEfmt ", 8);
    v = 16; memcpy(h + 16, &v, 4); uint16_t s = (uint16_t)fmt; memcpy(h + 20, &s, 2); s = (uint16_t)chans; memcpy(h + 22, &s, 2);
    v = rate; memcpy(h + 24, &v, 4); v = rate * chans / 2; memcpy(h + 28, &v, 4); s = (uint16_t)(chans == 2 ? 1 : 1); memcpy(h + 32, &s, 2);
    s = 4; memcpy(h + 34, &s, 2); memcpy(h + 36, "data", 4); memcpy(h + 40, &bytes, 4);
    fwrite(h, 1, 44, f); fwrite(data, 1, bytes, f); fclose(f);
}
/* aicaenc IN.wav OUT.wav [--loop LS,LE]: PCM16 mono/stereo -> AICA ADPCM
   (mono fmt 0x14, stereo fmt 0x20 interleaved); prints JSON per channel */
static int cmd_aicaenc(int argc, char **argv) {
    if (argc < 4) die("aicaenc IN OUT [--loop LS,LE]");
    long ls = -1, le = -1;
    for (int i = 4; i < argc; i++) if (!strcmp(argv[i], "--loop") && i + 1 < argc) { sscanf(argv[++i], "%ld,%ld", &ls, &le); }
    FILE *f = fopen(argv[2], "rb"); if (!f) die("open in");
    fseek(f, 0, SEEK_END); long n = ftell(f); fseek(f, 0, SEEK_SET);
    uint8_t *d = malloc(n); if (fread(d, 1, n, f) != (size_t)n) die("read"); fclose(f);
    int chans = rd16(d + 22); uint32_t rate = rd32(d + 24);
    if (rd16(d + 20) != 1 || rd16(d + 34) != 16) die("aicaenc needs PCM16");
    long p = 12; uint32_t dsz = 0; int16_t *pcm = NULL;
    while (p + 8 <= n) { uint32_t cs = rd32(d + p + 4); if (!memcmp(d + p, "data", 4)) { pcm = (int16_t *)(d + p + 8); dsz = cs; break; } p += 8 + cs + (cs & 1); }
    if (!pcm) die("no data chunk");
    long ns = dsz / 2 / chans;
    long nsEven = (ns + 1) & ~1L;
    uint8_t *codes[2] = {calloc(nsEven + 1, 1), calloc(nsEven + 1, 1)};
    char rep[2][512];
    for (int c = 0; c < chans; c++) adpcm_encode_channel(pcm + c, chans, ns, ls, le, codes[c], rep[c], sizeof rep[c]);
    uint32_t bytes; uint8_t *out;
    if (chans == 1) { bytes = nsEven / 2; out = calloc(bytes, 1); for (long i = 0; i < nsEven; i += 2) out[i / 2] = (uint8_t)(codes[0][i] | codes[0][i + 1] << 4); }
    else { bytes = ns; out = calloc(bytes, 1); for (long i = 0; i < ns; i++) out[i] = (uint8_t)(codes[0][i] << 4 | codes[1][i]); }
    write_wav_fmt(argv[3], out, bytes, chans, rate, chans == 2 ? 0x20 : 0x14);
    printf("{\"samples\":%ld,\"channels\":%d,\"bytes\":%u,\"ch\":[%s%s%s]}\n", ns, chans, bytes, rep[0], chans == 2 ? "," : "", chans == 2 ? rep[1] : "");
    return 0;
}

int main(int argc, char **argv) {
    if (argc < 2) die("usage: sseqrender render|swar ...");
    if (!strcmp(argv[1], "render")) return cmd_render(argc, argv);
    if (!strcmp(argv[1], "swar")) return cmd_swar(argc, argv);
    if (!strcmp(argv[1], "aicadec")) return cmd_aicadec(argc, argv);
    if (!strcmp(argv[1], "aicaenc")) return cmd_aicaenc(argc, argv);
    if (!strcmp(argv[1], "strm")) return cmd_strm(argc, argv);
    die("unknown command");
    return 1;
}
