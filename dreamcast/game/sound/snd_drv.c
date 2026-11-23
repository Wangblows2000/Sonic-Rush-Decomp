/*
 * Native sound driver: the DS ARM7 SND sequence driver, run on the SH-4.
 *
 * Source: tools/dreamcast/audio/sseqrender.c (this project's offline renderer,
 * written from the SDAT format notes and the NitroSDK driver behaviour; it is
 * the renderer that produced every music/SFX file of the port, validated in
 * tools/dreamcast/audio/validate_audio.py). The sequencer (track_step),
 * envelopes, LFO, sweep, channel allocation and the register formulas are
 * reused unchanged; what changed:
 *   - one global player -> 16 sequence players sharing a pool of 32 tracks
 *     (NitroSDK SND_PLAYER_NUM / SND_TRACK_NUM), with SND_PrepareSeq /
 *     SND_StartPreparedSeq / SND_StopSeq / SND_SetPlayerVolume /
 *     SND_SetPlayerChannelPriority / SND_SetTrackPan entry points;
 *   - local variables 0-15 per player, 16-31 global (NitroSDK layout);
 *   - track extPan (SND_SetTrackPan) added to the track pan;
 *   - pitch table read from the bank (computed by sfxbank.py with the same
 *     formula) instead of pow();
 *   - no mixer: the per-channel registers (wave, volume, shift, pan, timer)
 *     go to 16 AICA channels (snd_out_*.c). The hardware channel model only
 *     keeps the playback position so a one-shot wave ends on the same driver
 *     frame as on the DS;
 *   - loop detection / note logging (offline-only) removed.
 * PSG (square) and noise instruments play generated waves (bank psg_wave /
 * noise_wave) at the same timer.
 */
#include "snd_internal.h"
#include <stdio.h>
#include <string.h>

#define FRAME_SAMPLES_X2 341 /* 170.5 output samples per frame */
#define AMPL_MIN (-92544)

static const SndBank *s_bank;
static const SrbTables *T;

/* ---------------- bank access (instrument / wave lookup) ---------------- */
static uint32_t rd32(const uint8_t *p) { return p[0] | (p[1] << 8) | (p[2] << 16) | ((uint32_t)p[3] << 24); }
static uint16_t rd16(const uint8_t *p) { return (uint16_t)(p[0] | (p[1] << 8)); }

typedef struct { int type; int wave, swar, duty, rootKey, attack, decay, sustain, release, pan; } Inst;
static void parse_param(const uint8_t *p, int type, Inst *o) {
    o->type = type; o->wave = rd16(p); o->swar = rd16(p + 2); o->duty = rd16(p);
    o->rootKey = p[4]; o->attack = p[5]; o->decay = p[6]; o->sustain = p[7]; o->release = p[8]; o->pan = p[9];
}
static int read_inst(int bankNo, int prg, int key, Inst *o) {
    uint32_t bankSize;
    const uint8_t *bank = snd_bank_sbnk(s_bank, bankNo, &bankSize);
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
/* bank wave index of SWAR slot `swar`, wave `idx` (-1 = missing) */
static int get_wave(int bankNo, int swar, int idx) {
    const SrbBank *b;
    if (bankNo < 0 || (uint32_t)bankNo >= s_bank->hdr->n_banks || swar < 0 || swar > 3) return -1;
    b = &s_bank->banks[bankNo];
    if (b->wave_base[swar] < 0 || idx >= b->wave_count[swar]) return -1;
    if (s_bank->waves[b->wave_base[swar] + idx].samples == 0) return -1;
    return b->wave_base[swar] + idx;
}

/* ---------------- channels ---------------- */
enum { ENV_ATTACK, ENV_DECAY, ENV_SUSTAIN, ENV_RELEASE };
typedef struct { int target, speed, depth, range, delay; } LfoParam;
struct Track;
typedef struct {
    /* ExChannel (driver) state */
    int active, start, type; /* type 0 pcm 1 psg 2 noise */
    int prio; struct Track *track; int nextInTrack;
    int wave; int duty; int baseTimer;
    int key, rootKey, velocity, initPan;
    int envStatus, envAttn, envAttack, envDecay, envSustain, envRelease;
    int length;
    int sweepPitch, sweepLength, sweepCounter, autoSweep;
    int userDecay, userDecay2, userPitch, userPan, panRange;
    LfoParam lfo; int lfoCounter, lfoDelayCnt;
    int volume; /* last computed volume register (vol | shift<<8) */
    int regTimer, regPan;
    int stopReq; /* stop at next register update */
} ExChan;
typedef struct {
    int active; int type; int wave; int duty;
    uint64_t pos; int vol, shift, pan, timer;
} HwChan;
static ExChan ch[SND_CH_NUM]; static HwChan hw[SND_CH_NUM];
static SndChanRegs regs[SND_CH_NUM];
static SndDrvStats stats;

typedef struct Track {
    int alloc, active; uint32_t pc;
    int wait, noteWait, noteFinishWait, tie, porta, portaKey, portaTime, sweepPitch, muted;
    int vol, expr, pan, panRange, transpose, bend, bendRange, prio, prg;
    int attack, decay, sustain, release;
    LfoParam mod; int cmp;
    uint32_t stack[3]; int loopCnt[3]; int depth;
    int chanHead; /* index of first channel, -1 none */
    int extPan;   /* SND_SetTrackPan */
    struct Player *player;
} Track;

typedef struct Player {
    int active, started, paused;   /* paused: SND_PauseSeq */
    int notesTry, notesOk;         /* trace: notes of this sequence that looked up / started a channel */
    const uint8_t *seqData; uint32_t seqLen;
    int bankNo;
    int trackIdx[16];
    int volume, extFader, prio, tempo, tempoRatio, tempoCounter;
    int chanMask;
    int16_t vars[16];
} Player;

static Track tracks[SND_TRACK_NUM];
static Player players[SND_PLAYER_NUM];
static int16_t gvars[16];
static uint32_t rnd = 0x12345678;
static long frameNo = 0;

static int calc_random(void) { rnd = rnd * 1664525u + 1013904223u; return rnd >> 16; }
static int sin_idx(int x) {
    if (x < 0x20) return T->sine[x];
    if (x < 0x40) return T->sine[0x40 - x];
    if (x < 0x60) return -T->sine[x - 0x40];
    return -T->sine[0x20 - (x - 0x60)];
}
static int attack_coef(int a) { return a < 109 ? 255 - a : T->attack[127 - a]; }
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
    uint64_t r = T->pitch[p];
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
    int v = T->volume[dB + 723], s = dB < -240 ? 3 : dB < -120 ? 2 : dB < -60 ? 1 : 0;
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
        int c = T->order[i];
        if (!(mask & (1 << c))) continue;
        if (best < 0) { best = c; continue; }
        if (ch[c].prio > ch[best].prio) continue;
        if (ch[c].prio != ch[best].prio || vol_cmp(&ch[best], &ch[c]) < 0) best = c;
    }
    if (best < 0) return -1;
    if (prio < ch[best].prio) return -1;
    if (ch[best].active) stats.steals++;
    track_unlink(best);
    ExChan *e = &ch[best];
    memset(e, 0, sizeof *e);
    e->prio = prio; e->volume = 127; e->autoSweep = 1; e->key = 60; e->rootKey = 60; e->velocity = 127;
    e->panRange = 127; e->envAttack = attack_coef(127); e->envDecay = decay_coef(127);
    e->envSustain = 127; e->envRelease = decay_coef(127); lfo_init(&e->lfo);
    e->nextInTrack = -1; e->wave = -1;
    hw[best].active = 0; e->stopReq = 0;
    return best;
}
static int note_on(int c, int bankNo, int key, int vel, int length, const Inst *in) {
    ExChan *e = &ch[c];
    int release = in->release;
    if (release == 0xFF) { length = -1; release = 0; }
    switch (in->type) {
    case 1: case 4: {
        int w = get_wave(bankNo, in->swar, in->wave);
        if (w < 0) return 0;
        e->type = 0; e->wave = w; e->baseTimer = s_bank->waves[w].timer; break;
    }
    case 2: if (c < 8 || c > 13) return 0; e->type = 1; e->duty = in->duty & 7;
        e->wave = (int)s_bank->hdr->psg_wave + e->duty; e->baseTimer = 8006; break;
    case 3: if (c < 14) return 0; e->type = 2; e->wave = (int)s_bank->hdr->noise_wave; e->baseTimer = 8006; break;
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
    Player *pl = t->player; int alloc = t->alloc;
    memset(t, 0, sizeof *t); t->player = pl; t->alloc = alloc;
    t->noteWait = 1; t->cmp = 1; t->prio = 64; t->vol = 127; t->expr = 127; t->panRange = 127; t->bendRange = 2;
    t->portaKey = 60; t->attack = t->decay = t->sustain = t->release = 0xFF; lfo_init(&t->mod);
    t->chanHead = -1;
}
static void track_release_all(Track *t, int rel) {
    for (int c = t->chanHead; c >= 0; c = ch[c].nextInTrack) release_chan(c, rel);
}
static void track_free_chans(Track *t) {
    while (t->chanHead >= 0) { int c = t->chanHead; t->chanHead = ch[c].nextInTrack; ch[c].track = NULL; ch[c].nextInTrack = -1; }
}
static void track_stop(Track *t) { track_release_all(t, -1); track_free_chans(t); t->active = 0; }

static int rd8s(Track *t) { const Player *p = t->player; return t->pc < p->seqLen ? p->seqData[t->pc++] : 0xFF; }
static int16_t *var_ptr(Player *p, int i) { return i < 16 ? &p->vars[i] : &gvars[i - 16]; }
static int read_arg(Track *t, int type) {
    int v = 0;
    switch (type) {
    case 0: v = rd8s(t); break;                                         /* u8 */
    case 1: v = rd8s(t); v |= rd8s(t) << 8; v = (int16_t)v; break;     /* s16 */
    case 2: { int b, n = 0; do { b = rd8s(t); v = (v << 7) | (b & 0x7F); } while ((b & 0x80) && ++n < 4); break; }
    case 3: { int lo = rd8s(t); lo |= rd8s(t) << 8; lo = (int16_t)lo; int hi = rd8s(t); hi |= rd8s(t) << 8; hi = (int16_t)hi;
              v = ((calc_random() * (hi - lo + 1)) >> 16) + lo; break; }
    case 4: { int i = rd8s(t); if (i < 32) v = *var_ptr(t->player, i); break; }
    }
    return v;
}
static void play_note(Track *t, int key, int vel, int length) {
    Player *p = t->player;
    int c = -1;
    if (t->tie && t->chanHead >= 0) { c = t->chanHead; ch[c].key = key; ch[c].velocity = vel; }
    if (c < 0) {
        Inst in;
        p->notesTry++;
        if (!read_inst(p->bankNo, t->prg, key, &in)) { stats.inst_missing++; return; }
        int allowed;
        switch (in.type) { case 1: case 4: allowed = 0xFFFF; break; case 2: allowed = 0x3F00; break; case 3: allowed = 0xC000; break; default: return; }
        allowed &= p->chanMask;
        int pr = p->prio + t->prio; if (pr > 255) pr = 255;
        c = alloc_chan(allowed, pr);
        if (c < 0) { stats.alloc_fail++; return; }
        if (!note_on(c, p->bankNo, key, vel, t->tie ? -1 : length, &in)) { stats.wave_missing++; ch[c].prio = 0; ch[c].active = 0; return; }
        ch[c].track = t; ch[c].nextInTrack = t->chanHead; t->chanHead = c;
        stats.notes++; p->notesOk++;
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
    track_init(t); t->active = 1; t->pc = off;
}
static Track *player_track(Player *p, int tn) {
    return (tn >= 0 && tn < 16 && p->trackIdx[tn] >= 0) ? &tracks[p->trackIdx[tn]] : NULL;
}

/* returns 1 if track ended */
static int track_step(Track *t) {
    Player *p = t->player;
    for (int c = t->chanHead; c >= 0; c = ch[c].nextInTrack) {
        if (ch[c].length > 0) ch[c].length--;
        if (!ch[c].autoSweep && ch[c].sweepCounter < ch[c].sweepLength) ch[c].sweepCounter++;
    }
    if (t->noteFinishWait) { if (t->chanHead >= 0) return 0; t->noteFinishWait = 0; }
    if (t->wait > 0) { t->wait--; if (t->wait > 0) return 0; }
    int guard = 0;
    while (t->wait == 0 && !t->noteFinishWait) {
        if (++guard > 20000) return 1; /* runaway loop with no wait: treat as end */
        if (t->pc >= p->seqLen) return 1;
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
            int a = read_arg(t, useArg ? argType : 2);
            if (!run) break;
            if (cmd == 0x80) t->wait = a;
            else if (cmd == 0x81) { if (a < 0x10000) t->prg = a; }
            break;
        }
        case 0x90: {
            if (cmd == 0x93) {
                int tn = rd8s(t); uint32_t off = rd8s(t); off |= rd8s(t) << 8; off |= rd8s(t) << 16;
                Track *o = player_track(p, tn);
                if (run && o && o != t) { track_stop(o); track_start(o, off); }
            } else if (cmd == 0x94 || cmd == 0x95) {
                uint32_t off = rd8s(t); off |= rd8s(t) << 8; off |= rd8s(t) << 16;
                if (!run) break;
                if (cmd == 0x94) t->pc = off;
                else if (t->depth < 3) { t->stack[t->depth++] = t->pc; t->pc = off; }
            }
            break;
        }
        case 0xC0: case 0xD0: {
            int a = read_arg(t, useArg ? argType : 0);
            if (!run) break;
            switch (cmd) {
            case 0xC0: t->pan = (int8_t)(a - 0x40); break;
            case 0xC1: t->vol = a & 0xFF; break;
            case 0xC2: p->volume = a & 0xFF; break;
            case 0xC3: t->transpose = (int8_t)a; break;
            case 0xC4: t->bend = (int8_t)a; break;
            case 0xC5: t->bendRange = a & 0xFF; break;
            case 0xC6: t->prio = a & 0xFF; break;
            case 0xC7: t->noteWait = a & 1; break;
            case 0xC8: t->tie = a & 1; track_release_all(t, -1); track_free_chans(t); break;
            case 0xC9: t->portaKey = (a + t->transpose) & 0xFF; t->porta = 1; break;
            case 0xCA: t->mod.depth = a & 0xFF; break;
            case 0xCB: t->mod.speed = a & 0xFF; break;
            case 0xCC: t->mod.target = a & 0xFF; break;
            case 0xCD: t->mod.range = a & 0xFF; break;
            case 0xCE: t->porta = a & 1; break;
            case 0xCF: t->portaTime = a & 0xFF; break;
            case 0xD0: t->attack = a & 0xFF; break;
            case 0xD1: t->decay = a & 0xFF; break;
            case 0xD2: t->sustain = a & 0xFF; break;
            case 0xD3: t->release = a & 0xFF; break;
            case 0xD4: if (t->depth < 3) { t->stack[t->depth] = t->pc; t->loopCnt[t->depth] = a & 0xFF; t->depth++; } break;
            case 0xD5: t->expr = a & 0xFF; break;
            case 0xD6: break;
            case 0xD7: t->muted = a & 1; if (a) { track_release_all(t, -1); } break;
            default: break;
            }
            break;
        }
        case 0xE0: {
            int a = (int16_t)read_arg(t, useArg ? argType : 1);
            if (!run) break;
            if (cmd == 0xE0) t->mod.delay = a & 0xFFFF;
            else if (cmd == 0xE1) p->tempo = a & 0xFFFF;
            else if (cmd == 0xE3) t->sweepPitch = a;
            break;
        }
        case 0xB0: {
            int vi = rd8s(t);
            int a = (int16_t)read_arg(t, useArg ? argType : 1);
            if (!run || vi >= 32) break;
            int16_t *v = var_ptr(p, vi);
            switch (cmd) {
            case 0xB0: *v = a; break;
            case 0xB1: *v += a; break;
            case 0xB2: *v -= a; break;
            case 0xB3: *v *= a; break;
            case 0xB4: if (a) *v /= a; break;
            case 0xB5: if (a >= 0) *v <<= a; else *v >>= -a; break;
            case 0xB6: { int neg = a < 0; if (neg) a = -a; int r = (calc_random() * (a + 1)) >> 16; *v = neg ? -r : r; break; }
            case 0xB8: t->cmp = *v == a; break;
            case 0xB9: t->cmp = *v >= a; break;
            case 0xBA: t->cmp = *v > a; break;
            case 0xBB: t->cmp = *v <= a; break;
            case 0xBC: t->cmp = *v < a; break;
            case 0xBD: t->cmp = *v != a; break;
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

/* SND_StopSeq / player end: stop tracks (channels go to release) and free them */
static void player_shutdown(Player *p) {
    for (int i = 0; i < 16; i++) {
        Track *t = player_track(p, i);
        if (!t) continue;
        track_stop(t);
        t->alloc = 0; t->player = NULL;
        p->trackIdx[i] = -1;
    }
    p->active = 0; p->started = 0; p->paused = 0;
}

static void player_step_ticks(Player *p) {
    int playing = 0;
    for (int i = 0; i < 16; i++) {
        Track *t = player_track(p, i);
        if (!t || !t->active) continue;
        if (track_step(t)) track_stop(t); else playing = 1;
    }
    if (!playing) player_shutdown(p);
}

static void track_update_channels(Track *t) {
    Player *p = t->player;
    int vol = T->dbsq[t->vol & 127] + T->dbsq[t->expr & 127] + T->dbsq[p->volume & 127];
    int fader = p->extFader;
    int pitch = (t->bend * (t->bendRange << 6)) >> 7;
    int pan = t->pan;
    if (t->panRange != 127) pan = (pan * t->panRange + 0x40) >> 7;
    pan += t->extPan;
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

static void seq_main(void) {
    for (int k = 0; k < SND_PLAYER_NUM; k++) {
        Player *p = &players[k];
        if (!p->active || !p->started || p->paused) continue;
        int ticks = 0;
        while (p->tempoCounter >= 240) { p->tempoCounter -= 240; ticks++; }
        for (int i = 0; i < ticks && p->active; i++) player_step_ticks(p);
        p->tempoCounter += (p->tempo * p->tempoRatio) >> 8;
    }
    for (int k = 0; k < SND_PLAYER_NUM; k++) {
        Player *p = &players[k];
        if (!p->active) continue;
        for (int i = 0; i < 16; i++) {
            Track *t = player_track(p, i);
            if (t && (t->active || t->chanHead >= 0)) track_update_channels(t);
        }
    }
}

static void exchannel_main(void) {
    int voices = 0;
    for (int i = 0; i < 16; i++) {
        ExChan *e = &ch[i];
        if (!e->active) continue;
        if (e->start) { e->start = 0; e->stopReq = 0; /* hw start applied at next frame */ hw[i].active = -1; }
        else if (!hw[i].active) { chan_finished(i); continue; }
        voices++;
        int vol = T->dbsq[e->velocity & 127];
        int pitch = (e->key - e->rootKey) * 64;
        /* envelope */
        switch (e->envStatus) {
        case ENV_ATTACK: e->envAttn = -((-e->envAttn * e->envAttack) >> 8); if (e->envAttn == 0) e->envStatus = ENV_DECAY; break;
        case ENV_DECAY: { int sus = T->dbsq[e->envSustain & 127] << 7; e->envAttn -= e->envDecay; if (e->envAttn <= sus) { e->envAttn = sus; e->envStatus = ENV_SUSTAIN; } break; }
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
    if (voices > stats.max_voices) stats.max_voices = voices;
}

/* apply registers computed in the previous frame (SND_UpdateExChannel) */
static void update_hw(void) {
    for (int i = 0; i < 16; i++) {
        ExChan *e = &ch[i]; HwChan *h = &hw[i];
        if (e->stopReq) { h->active = 0; e->stopReq = 0; }
        if (!e->active) { h->active = 0; continue; }
        if (h->active == -1) {
            h->active = 1; h->type = e->type; h->wave = e->wave; h->duty = e->duty; h->pos = 0;
            regs[i].starts++;
        }
        h->vol = e->volume & 0xFF; h->shift = SHIFT_TAB[(e->volume >> 8) & 3];
        h->pan = e->regPan; h->timer = e->regTimer ? e->regTimer : 0x10;
    }
}

/* playback position of the hardware channels over one frame (the position part
 * of sseqrender.c mix(): a one-shot wave stops at the first output sample whose
 * position is past its end) */
static void hw_advance(void) {
    long n = ((frameNo + 1) * FRAME_SAMPLES_X2) / 2 - (frameNo * FRAME_SAMPLES_X2) / 2;
    for (int i = 0; i < 16; i++) {
        HwChan *h = &hw[i];
        if (h->active != 1) continue;
        const SrbWave *w = &s_bank->waves[h->wave];
        uint64_t step = ((uint64_t)512 << 32) / (uint64_t)h->timer;
        if (h->type != 0) { h->pos += step * (uint64_t)n; continue; } /* PSG / noise: endless */
        uint64_t last = (h->pos + step * (uint64_t)(n - 1)) >> 32;
        if (last >= w->samples) {
            if (w->loop && w->samples > w->loop_start) {
                uint64_t ll = w->samples - w->loop_start, ip;
                h->pos += step * (uint64_t)n;
                ip = h->pos >> 32;
                if (ip >= w->samples) { ip = w->loop_start + (ip - w->loop_start) % ll; h->pos = (ip << 32) | (h->pos & 0xFFFFFFFFu); }
            } else {
                h->active = 0;
            }
        } else {
            h->pos += step * (uint64_t)n;
        }
    }
}

void snd_drv_frame(void) {
    update_hw();
    seq_main();
    exchannel_main();
    hw_advance();
    frameNo++;
    for (int i = 0; i < 16; i++) {
        regs[i].active = hw[i].active == 1;
        regs[i].wave = hw[i].wave; regs[i].vol = hw[i].vol; regs[i].shift = hw[i].shift;
        regs[i].pan = hw[i].pan; regs[i].timer = hw[i].timer;
    }
}

/* ---------------- output (AICA) ---------------- */
/* DS mixer: channel gain vol/128 / 2^shift, linear pan (128-pan)/128, pan/128,
 * master 127/128. AICA: the louder side at that amplitude (TL, 0.375 dB
 * steps), the other side attenuated in 3 dB steps (DIPAN, KOS pan value). The music
 * stream (engine/audio.c) plays the DS-model render at 255 hard left/right,
 * so this keeps effects and music at the DS level ratio.
 * Port mix gain: every channel is scaled by 2^(-mix_gain_tl/16) from the bank
 * header, the same gain sdat2dc.py baked into the music renders (one shared
 * gain: headroom for the DS mix, which clamps at the same full scale). */
static uint8_t panKos[128];
static uint32_t mixGain = 65536; /* q16 */
static volatile uint32_t loadFade = 65536; /* q16, audio_load_begin fade-out of the effects (1 = off) */
void snd_drv_set_fade(uint32_t q16) { loadFade = q16 > 65536 ? 65536 : q16; }
static uint32_t mix_gain_q16(uint32_t tl) {
    /* round(65536 * 2^(-i/16)), i = 0..15 */
    static const uint16_t frac[16] = {0, 62757, 60097, 57549, 55109, 52773, 50535, 48393,
                                      46341, 44376, 42495, 40693, 38968, 37316, 35734, 34219};
    uint32_t g = (tl & 15) ? frac[tl & 15] : 65536;
    return tl >= 16 * 17 ? 0 : g >> (tl >> 4);
}
static uint16_t panGain[128]; /* q8 */
static struct { int on; uint32_t starts; uint32_t freq; int vol, pan; } out[SND_CH_NUM];
uint32_t snd_drv_mix_gain(void) { return mixGain; }

static int ilog_steps(int num, int den) {
    /* round(20*log10(num/den)/3) without libm: count 3 dB steps (x 1.41254) */
    int s = 0;
    int64_t n = (int64_t)num * 100000, d = den;
    /* compare num/den against 10^((s+0.5)*3/20) */
    static const int64_t thr[16] = {118850, 167880, 237137, 334965, 473151, 668344, 944061, 1333521,
                                    1883649, 2660725, 3758374, 5308845, 7498942, 10592537, 14962357, 21134890};
    if (den == 0) return 15;
    while (s < 15 && n >= thr[s] * d) s++;
    return s;
}
static void out_tables(void) {
    for (int p = 0; p < 128; p++) {
        int gl = 128 - p, gr = p, m = gl > gr ? gl : gr, s;
        panGain[p] = (uint16_t)(m * 2); /* /128 -> q8 */
        if (gl == gr) { panKos[p] = 0x80; continue; }
        s = gl > gr ? ilog_steps(gl, gr) : ilog_steps(gr, gl);
        if (s == 0) panKos[p] = 0x80;
        else panKos[p] = (uint8_t)(gl > gr ? 0x7F - 8 * s : 0x80 + 8 * s);
    }
}
static uint32_t timer_to_freq(int timer) { return 16756991u / (uint32_t)(timer ? timer : 0x10); }
/* linear amplitude, q16 (65535 = full scale): vol/128 / 2^shift * 127/128 * gain/256 * mix gain */
static int out_vol(const HwChan *h) {
    int64_t v = ((int64_t)h->vol * 127 * panGain[h->pan] * 65535) >> 22;
    v = (v * mixGain) >> 16;
    v = (v * loadFade) >> 16;
    v >>= h->shift;
    return v > 65535 ? 65535 : (int)v;
}

void snd_drv_flush(void) {
    for (int i = 0; i < 16; i++) {
        HwChan *h = &hw[i];
        if (h->active == 1) {
            uint32_t f = timer_to_freq(h->timer);
            int v = out_vol(h), p = panKos[h->pan];
            if (f > 0x3FFFFF) f = 0x3FFFFF;
            if (!out[i].on || out[i].starts != regs[i].starts) {
                const SrbWave *w = &s_bank->waves[h->wave];
                snd_out_start(i, s_bank->aram + w->data_off, w->format, w->samples, w->loop, w->loop_start, f, v, p);
                out[i].on = 1; out[i].starts = regs[i].starts; out[i].freq = f; out[i].vol = v; out[i].pan = p;
            } else {
                unsigned what = (f != out[i].freq ? SND_OUT_FREQ : 0) | (v != out[i].vol ? SND_OUT_VOL : 0) |
                                (p != out[i].pan ? SND_OUT_PAN : 0);
                if (what) { snd_out_update(i, what, f, v, p); out[i].freq = f; out[i].vol = v; out[i].pan = p; }
            }
        } else if (out[i].on) {
            snd_out_stop(i);
            out[i].on = 0;
        }
    }
}

/* ---------------- entry points ---------------- */
void snd_drv_init(const SndBank *bank) {
    s_bank = bank; T = bank->tab;
    memset(ch, 0, sizeof ch); memset(hw, 0, sizeof hw); memset(tracks, 0, sizeof tracks);
    memset(players, 0, sizeof players); memset(regs, 0, sizeof regs); memset(&stats, 0, sizeof stats);
    for (int i = 0; i < 16; i++) {
        ch[i].nextInTrack = -1; ch[i].wave = -1;
        if (out[i].on) snd_out_stop(i);
        out[i].on = 0;
    }
    for (int k = 0; k < SND_PLAYER_NUM; k++) for (int i = 0; i < 16; i++) players[k].trackIdx[i] = -1;
    for (int i = 0; i < 16; i++) gvars[i] = -1;
    frameNo = 0;
    out_tables();
    mixGain = mix_gain_q16(bank->hdr->mix_gain_tl);
}

static int alloc_track(Player *p) {
    for (int i = 0; i < SND_TRACK_NUM; i++)
        if (!tracks[i].alloc) { tracks[i].alloc = 1; tracks[i].player = p; track_init(&tracks[i]); return i; }
    return -1;
}

void snd_drv_prepare_seq(int player, const uint8_t *seq, uint32_t seq_len, uint32_t offset, int bank_no) {
    Player *p = &players[player];
    if (p->active) player_shutdown(p);
    p->seqData = seq; p->seqLen = seq_len; p->bankNo = bank_no;
    p->notesTry = p->notesOk = 0;
    p->volume = 127; p->extFader = 0; p->prio = 64; p->tempo = 120; p->tempoRatio = 256; p->tempoCounter = 240;
    if (!p->chanMask) p->chanMask = 0xFFFF;
    for (int i = 0; i < 16; i++) p->vars[i] = -1;
    int t0 = alloc_track(p);
    if (t0 < 0) return;
    p->trackIdx[0] = t0;
    uint32_t pc = offset;
    if (pc + 3 <= seq_len && seq[pc] == 0xFE) {
        int mask = rd16(seq + pc + 1);
        pc += 3;
        for (int i = 1; i < 16; i++) if (mask & (1 << i)) p->trackIdx[i] = alloc_track(p);
    }
    track_start(&tracks[t0], pc);
    p->active = 1; p->started = 0;
}
/* SND_PauseSeq (ARM7 SNDi_PauseSeq): pausing releases every channel of the
 * player's tracks (release rate 127) and detaches them; the sequence stops
 * ticking until it is resumed (tracks keep their position and state). */
void snd_drv_pause_seq(int player, int flag) {
    Player *p = &players[player];
    if (!p->active) return;
    if (flag && !p->paused)
        for (int i = 0; i < 16; i++) {
            Track *t = player_track(p, i);
            if (t) { track_release_all(t, 127); track_free_chans(t); }
        }
    p->paused = flag ? 1 : 0;
}
void snd_drv_player_notes(int player, int *tries, int *ok) { *tries = players[player].notesTry; *ok = players[player].notesOk; }
void snd_drv_start_prepared(int player) { if (players[player].active) players[player].started = 1; }
void snd_drv_stop_seq(int player) { if (players[player].active) player_shutdown(&players[player]); }
int snd_drv_player_active(int player) { return players[player].active; }
void snd_drv_set_player_volume(int player, int db) { players[player].extFader = db; }
void snd_drv_set_player_prio(int player, int prio) { players[player].prio = prio; }
void snd_drv_set_alloc_mask(int player, uint16_t mask) { players[player].chanMask = mask ? mask : 0xFFFF; }
void snd_drv_set_track_pan(int player, uint32_t mask, int pan) {
    Player *p = &players[player];
    for (int i = 0; i < 16; i++) {
        Track *t = player_track(p, i);
        if (t && (mask & (1u << i))) t->extPan = (int8_t)pan;
    }
}
/* SND_SetPlayerTempoRatio: 256 = normal speed */
void snd_drv_set_tempo_ratio(int player, int ratio) { players[player].tempoRatio = ratio; }
void snd_drv_stop_all(void) {
    for (int k = 0; k < SND_PLAYER_NUM; k++) if (players[k].active) player_shutdown(&players[k]);
    for (int i = 0; i < 16; i++) { if (ch[i].active) chan_finished(i); hw[i].active = 0; }
    snd_drv_flush();
}
const SndChanRegs *snd_drv_regs(int c) { return &regs[c]; }

/* debug: summed worst-case amplitude of the active channels (q16 per side,
 * louder side; 65535 = full scale). Printed when it reaches -6 dBFS. */
void snd_drv_trace(uint32_t frame) {
    int sum = 0, n = 0;
    char buf[200]; int k = 0;
    for (int i = 0; i < 16; i++) {
        if (hw[i].active != 1) continue;
        int v = out_vol(&hw[i]);
        sum += v; n++;
        if (k < (int)sizeof buf - 24) k += snprintf(buf + k, sizeof buf - k, " %d:w%d/v%d", i, hw[i].wave, v >> 8);
    }
    buf[k] = 0;
    if (sum >= 32768) printf("snd f%lu sum %d n %d%s\n", (unsigned long)frame, sum, n, buf);
}
const SndDrvStats *snd_drv_stats(void) { return &stats; }
