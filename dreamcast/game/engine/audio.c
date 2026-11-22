#include "audio.h"
#include <kos.h>
#include <dc/sound/sound.h>
#include <dc/sound/stream.h>
#include <dc/sound/sfxmgr.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <kos/thread.h>
#include <dc/sound/aica_comm.h>
#include <dc/g2bus.h>
#include "../sound/sound.h"

/* Music: snd_stream in ADPCM mode (AICA 4-bit Yamaha ADPCM, "long stream"),
 * fed from an AICA-ADPCM WAV on disc (sdat2dc.py output, nibble-interleaved
 * stereo: one byte = one stereo sample frame). No SH-4 decoding.
 *
 * Disc reads are decoupled from the AICA refill:
 *   reader thread  disc -> RAM ring (AUDIO_RING_BYTES, ~12 s of music)
 *   poll thread    snd_stream_poll every 10 ms; the stream callback only
 *                  copies from the ring and never waits for the disc.
 * Why: the AICA ADPCM decoder carries its state (previous sample, step)
 * across the whole stream and has no leak. If a refill comes late, the AICA
 * loops over stale bytes, the decoder state no longer matches the encoder's
 * and the error stays as a DC offset / full-scale noise for the rest of the
 * track. With the refill inside the poll (the previous design) a single
 * fs_read could block for seconds while the main thread loaded a large file
 * (fs_iso9660 serialises reads): measured in Flycast, 5.7 s and 6.1 s
 * blocks during/after the z11 stage load, which put full-scale bursts and a
 * permanent DC offset into the music (docs/dreamcast/AUDIO.md, "Mix level and
 * clipping"). audio_music_play only opens the file; the reader fills the ring
 * in the background and the AICA stream is started (by the poll thread) once
 * the ring holds AUDIO_RESTART bytes AND either the game loop runs again
 * (audio_update() called after audio_music_play: the stage load is over) or
 * the disc is not contended (last 16 KB read < 300 ms, e.g. in the menus, whose
 * loop does not call audio_update). In Flycast the z11 load right after
 * start_zone_music() kept the disc busy for ~17 s (one 16 KB read waited
 * 16.7 s), more than any ring could cover if the music had already started.
 * If the ring still runs dry, the stream is stopped (silence, not garbage) and
 * restarted from the start of the file, the only point where the AICA
 * decoder's reset state (0, step 127) matches the encoder's; underruns are
 * counted (audio_stream_underruns).
 *
 * Layers (.lyr, tools/dreamcast/audio/musictab.py): Stage_PlayBGM starts two
 * sequences (the character's track and its alternate mix: Water Palace
 * underwater, Dead Line gravity flip) and Stage_SelectBGMLayer only moves
 * their volumes. Here both layers are AICA streams fed in lockstep from one
 * file (32 KB of each layer in turn: one sequential read), each with its own
 * NNS-style volume fader, so a layer switch keeps the track position like the
 * DS. The two streams are keyed on back to back (KOS 2.3 cannot queue two
 * stereo stream starts: 6-13 ms apart in Flycast), then layer_resync() keys
 * all four channels off and on with one sync key-on, so both restart together
 * from file position 0 (AUDIO.md 14: no measurable offset after it).
 *
 * Jingles (StageJingle_Create: SEQ_muteki / SEQ_brethless, Sound_PlayBGMOnPlayer3:
 * SEQ_1up): short files held in RAM and decoded on the SH-4 into a PCM16
 * stream (2 slots). PCM has no decoder state in the AICA, so a jingle can be
 * paused and resumed at its position (StageJingle_Main pauses SEQ_brethless
 * while the game is frozen; the pause menu pauses players 3..9), which an
 * ADPCM stream cannot do; a RAM jingle also never touches the disc while the
 * stage music streams. Cost: ~65k decoded frames/s per slot. */
#define AUDIO_RING_BYTES (384 * 1024)   /* 12.0 s of stereo ADPCM at 32728 Hz (6 s per layer for 2) */
#define AUDIO_CHUNK (64 * 1024)         /* reader request size */
#define AUDIO_RESTART (128 * 1024)      /* ring level (all layers) needed to start / restart */
/* AICA ring per channel (KOS refills half of it per snd_stream_poll):
 * 16 KB = 0.5 s halves, refill CPU time max ~4 ms in the poll thread; still
 * 0.5 s of margin if the poll thread is late. (Tested 4 KB and the KOS maximum
 * 32704 B: no measurable effect on game-loop frame times in Flycast.) */
#ifndef AUDIO_AICA_BUF
#define AUDIO_AICA_BUF 16384
#endif
#ifndef AUDIO_AICA_BUF_L1
#define AUDIO_AICA_BUF_L1 4096          /* layer 1 of a .lyr: 0.125 s halves */
#endif
#ifndef AUDIO_READER_PRIO
#define AUDIO_READER_PRIO (PRIO_DEFAULT + 2)  /* below the game loop: reads run in its idle time */
#endif
#define AUDIO_JBUF 8192                 /* PCM16 jingle stream: AICA bytes per channel (0.125 s, 62 ms halves) */
#define AUDIO_JRING 16384               /* decoded PCM ring per jingle slot (frames, 0.5 s) */
#define AUDIO_JPREFILL 8192             /* ring level (frames) before the AICA stream starts */
#define AUDIO_JCHUNK 1024               /* frames decoded per s_lock hold (reader thread) */
#define AUDIO_JCP 4096                  /* jingle decoder checkpoint interval (frames) */
#define AUDIO_JCACHE 4
#define MAXL 2

static snd_stream_hnd_t s_hnd[MAXL] = {SND_STREAM_INVALID, SND_STREAM_INVALID};
static int s_nl = 1;                    /* layers of the open file */
static uint32_t s_lyr_block;            /* .lyr bytes per layer per block (0: plain WAV) */
static file_t s_fd = FILEHND_INVALID;
static uint32_t s_data_off, s_data_len, s_pos, s_loop_start_b, s_loop_end_b;
static int s_stereo, s_rate;
static volatile uint32_t s_streamed, s_loops, s_underruns;
static uint8_t s_ring[AUDIO_RING_BYTES] __attribute__((aligned(32)));
static uint32_t s_ringl = AUDIO_RING_BYTES;       /* ring bytes per layer */
static volatile uint32_t s_wr, s_rd[MAXL];        /* bytes written (all layers in lockstep) / consumed per layer */
static uint8_t s_buf[SND_STREAM_BUFFER_MAX_ADPCM * 2] __attribute__((aligned(32)));
/* debug trace (serial), on when /cd/debug/sndtrace.txt exists: poll gaps or
 * polls over 100 ms, poll errors, short reads, underruns, ring low-water mark */
static int s_trace;
static uint32_t s_low = AUDIO_RING_BYTES;
static volatile int s_dry;              /* underrun seen: poll thread stops the stream */
uint32_t audio_stream_bytes(void) { return s_streamed; }
uint32_t audio_stream_loops(void) { return s_loops; }
uint32_t audio_stream_underruns(void) { return s_underruns; }

static mutex_t s_lock = MUTEX_INITIALIZER;   /* stream handles, faders, jingles (poll thread / API) */
static mutex_t s_flock = MUTEX_INITIALIZER;  /* file + ring writer state (reader thread / API) */

/* ------------------------------------------------------------ faders
 * NNSi_SndFader: linear move of the NNS volume (0..127, amplitude v/127 through
 * the linear decibel table) over a number of 60 Hz frames; evaluated in ms by
 * the poll thread. */
typedef struct { int from, to; uint64_t t0; uint32_t dur; } Fader;
static int fader_get(const Fader *f, uint64_t now)
{
    if(!f->dur || now >= f->t0 + f->dur) return f->to;
    return f->from + (int)((int64_t)(f->to - f->from) * (int64_t)(now - f->t0) / (int64_t)f->dur);
}
static void fader_set(Fader *f, int to, int frames, uint64_t now)
{
    int cur = fader_get(f, now);
    if(to < 0) to = 0; else if(to > 127) to = 127;
    f->from = cur; f->to = to; f->t0 = now; f->dur = frames > 0 ? (uint32_t)frames * 1000u / 60u : 0;
}
static int fader_done(const Fader *f, uint64_t now) { return !f->dur || now >= f->t0 + f->dur; }

static Fader s_lfader[MAXL];
static int s_mstop;                     /* music: stop when the faders reach 0 (audio_music_fade_stop) */
static int s_lsent[MAXL] = {-1, -1};
static volatile uint32_t s_gen[3];

static int read_wav_header(file_t f, int *rate, int *ch, uint32_t *doff, uint32_t *dlen)
{
    uint8_t h[12];
    uint32_t id, sz;
    if(fs_read(f, h, 12) != 12 || memcmp(h, "RIFF", 4) || memcmp(h + 8, "WAVE", 4)) return -1;
    for(;;) {
        uint8_t c[8];
        if(fs_read(f, c, 8) != 8) return -1;
        memcpy(&id, c, 4); memcpy(&sz, c + 4, 4);
        if(!memcmp(c, "fmt ", 4)) {
            uint8_t fmt[16];
            fs_read(f, fmt, 16);
            *ch = fmt[2] | fmt[3] << 8;
            *rate = fmt[4] | fmt[5] << 8 | fmt[6] << 16 | fmt[7] << 24;
            fs_seek(f, sz - 16, SEEK_CUR);
        } else if(!memcmp(c, "data", 4)) {
            *doff = fs_tell(f); *dlen = sz;
            return 0;
        } else
            fs_seek(f, sz, SEEK_CUR);
    }
}

/* disc -> ring, at most `max` bytes per layer; s_flock held. Loops the file at
 * loop_end -> loop_start (whole file if no loop). Returns bytes read per layer.
 * Reads are whole 2048-byte sectors from a sector-aligned file offset into a
 * 32-byte aligned block, so fs_iso9660 does one multi-sector DMA read
 * (cdrom_read_sectors_ex) per call. Unaligned reads (the WAV data starts at
 * byte 44) went through its one-sector cache: one GD-ROM command per 2 KB and
 * a ~25-30 ms stall of the game loop per read in Flycast.
 * .lyr: block k holds [layer 0: s_lyr_block bytes][layer 1: s_lyr_block bytes]
 * at s_data_off + k * 2 * s_lyr_block (s_data_off = 2048), so one read of
 * AUDIO_CHUNK = 2 * 32 KB brings the same track range of both layers. */
static uint8_t s_blk[AUDIO_CHUNK] __attribute__((aligned(32)));
static uint32_t ring_used(void)
{
    uint32_t m = s_wr - s_rd[0];
    if(s_nl > 1 && s_wr - s_rd[1] > m) m = s_wr - s_rd[1];
    return m;
}
static uint32_t fill_ring(uint32_t max)
{
    uint32_t done = 0;
    uint32_t end = s_loop_end_b && s_loop_end_b <= s_data_len ? s_loop_end_b : s_data_len;
    while(s_fd != FILEHND_INVALID && done < max) {
        uint32_t room = s_ringl - ring_used(), at, n, fpos, blk, skip, first, avail;
        int r, L;
        if(room == 0) break;
        if(s_pos >= end) {
            s_pos = s_loop_start_b < end ? s_loop_start_b : 0;
            s_loops++;
        }
        if(s_lyr_block) {
            uint32_t k = s_pos / s_lyr_block;
            blk = s_data_off + k * (uint32_t)s_nl * s_lyr_block;
            skip = s_pos % s_lyr_block;
            fs_seek(s_fd, blk, SEEK_SET);
            r = fs_read(s_fd, s_blk, s_lyr_block * (uint32_t)s_nl);
            if(r < (int)(s_lyr_block * (uint32_t)s_nl)) {
                if(s_trace) printf("strm short read %d at %lu\n", r, (unsigned long)blk);
                break;
            }
            avail = s_lyr_block - skip;
        } else {
            fpos = s_data_off + s_pos;
            blk = fpos & ~2047u;
            skip = fpos - blk;
            fs_seek(s_fd, blk, SEEK_SET);
            r = fs_read(s_fd, s_blk, AUDIO_CHUNK);
            if(r <= (int)skip) {
                if(s_trace) printf("strm short read %d at %lu\n", r, (unsigned long)blk);
                break;
            }
            avail = (uint32_t)r - skip;
        }
        n = avail;
        if(n > max - done) n = max - done;
        if(n > room) n = room;
        if(n > end - s_pos) n = end - s_pos;
        at = s_wr % s_ringl;
        first = s_ringl - at < n ? s_ringl - at : n;
        for(L = 0; L < s_nl; L++) {
            uint8_t *ring = s_ring + (uint32_t)L * s_ringl;
            const uint8_t *src = s_blk + (s_lyr_block ? (uint32_t)L * s_lyr_block : 0) + skip;
            memcpy(ring + at, src, first);
            if(n > first) memcpy(ring, src + first, n - first);
        }
        s_wr += n; s_pos += n; done += n;
    }
    return done;
}

/* snd_stream callback (poll thread, s_lock held): ring -> AICA, never blocks */
static void *stream_cb(snd_stream_hnd_t hnd, int req, int *got)
{
    int L = (s_nl > 1 && hnd == s_hnd[1]) ? 1 : 0;
    uint32_t avail = s_wr - s_rd[L], n, at, first;
    const uint8_t *ring = s_ring + (uint32_t)L * s_ringl;
    if(req > (int)sizeof(s_buf)) req = sizeof(s_buf);
    req &= ~31;
    n = (uint32_t)req < avail ? (uint32_t)req : avail & ~31u;
    if(n < (uint32_t)req) {
        s_underruns++; s_dry = 1;
        if(s_trace) printf("strm underrun (layer %d): %lu of %d bytes in ring\n", L, (unsigned long)avail, req);
    }
    at = s_rd[L] % s_ringl;
    first = s_ringl - at < n ? s_ringl - at : n;
    memcpy(s_buf, ring + at, first);
    if(n > first) memcpy(s_buf + first, ring, n - first);
    s_rd[L] += n;
    if(s_wr - s_rd[L] < s_low) s_low = s_wr - s_rd[L];
    if(L == 0) s_streamed += n;
    *got = (int)n;
    return s_buf;
}

/* ------------------------------------------------------------ RAM jingles */
typedef struct {
    char path[48];
    uint8_t *blk;               /* the whole file */
    uint8_t *data;              /* fmt 0x20 stereo ADPCM: byte = L nibble << 4 | R nibble */
    uint32_t frames, rate;
    int16_t (*cp)[4];           /* decoder state every AUDIO_JCP frames: hist L, step L, hist R, step R */
    uint32_t ncp;
} JEntry;
typedef struct {
    JEntry *e;
    snd_stream_hnd_t hnd;
    uint32_t ls, le;            /* loop (frames), le = 0: no loop */
    uint32_t pos;               /* next frame to decode (file position) */
    uint64_t lin;               /* frames decoded since the play position `base` */
    int hist[2], step[2];
    int active, paused, ended, start_pending;
    int16_t pcm[AUDIO_JRING * 2] __attribute__((aligned(32)));  /* decoded ring, interleaved L/R */
    uint32_t pwr, prd;          /* frames written (reader thread) / consumed (stream callback) */
    uint32_t underruns;
    uint64_t base;              /* linear position (frames) at the last (re)start */
    uint64_t t_start;           /* ms of the last (re)start */
    Fader f; int stop_at_zero; int sent;
} JSlot;
static AudioStats s_st;
static JEntry s_jc[AUDIO_JCACHE];
static JSlot s_js[AUDIO_JINGLES];
static int16_t s_jbuf[AUDIO_JBUF * 2] __attribute__((aligned(32)));   /* callback buffer: interleaved L/R (KOS splits it) */
static const int k_step[8] = {230, 230, 230, 230, 307, 409, 512, 614};

static inline int adpcm_step(int *hist, int *step, int nib)
{
    int d = nib & 7, diff = ((1 + (d << 1)) * *step) >> 3;
    int h = (nib & 8) ? *hist - diff : *hist + diff;
    if(h > 32767) h = 32767; else if(h < -32768) h = -32768;
    *hist = h;
    d = (k_step[d] * *step) >> 8;
    *step = d < 127 ? 127 : d > 24576 ? 24576 : d;
    return h;
}

static JEntry *jcache_find(const char *path)
{
    int i;
    for(i = 0; i < AUDIO_JCACHE; i++) if(s_jc[i].blk && !strcmp(s_jc[i].path, path)) return &s_jc[i];
    return NULL;
}
static int jentry_in_use(const JEntry *e)
{
    int i;
    for(i = 0; i < AUDIO_JINGLES; i++) if(s_js[i].active && s_js[i].e == e) return 1;
    return 0;
}
static void jentry_free(JEntry *e)
{
    free(e->blk); free(e->cp);
    memset(e, 0, sizeof(*e));
}

/* read a stereo fmt 0x20 AICA-ADPCM WAV into the cache (blocking disc read) */
static JEntry *jcache_load(const char *path)
{
    JEntry *e = jcache_find(path), *slot = NULL;
    file_t f;
    int rate = 0, ch = 0, i;
    uint32_t doff = 0, dlen = 0, k;
    if(e) return e;
    for(i = 0; i < AUDIO_JCACHE && !slot; i++) if(!s_jc[i].blk) slot = &s_jc[i];
    for(i = 0; i < AUDIO_JCACHE && !slot; i++) if(!jentry_in_use(&s_jc[i])) { jentry_free(&s_jc[i]); slot = &s_jc[i]; }
    if(!slot) return NULL;
    f = fs_open(path, O_RDONLY);
    if(f == FILEHND_INVALID) return NULL;
    if(read_wav_header(f, &rate, &ch, &doff, &dlen) || ch != 2) { fs_close(f); return NULL; }
    /* one read of the whole file from offset 0 into a 32-byte aligned block: a
     * multi-sector DMA read (an unaligned start goes through fs_iso9660's
     * one-sector cache, one GD-ROM command per 2 KB) */
    slot->blk = memalign(32, ((doff + dlen + 2047) & ~2047u));
    if(!slot->blk) { fs_close(f); return NULL; }
    fs_seek(f, 0, SEEK_SET);
    if(fs_read(f, slot->blk, doff + dlen) != (ssize_t)(doff + dlen)) { fs_close(f); jentry_free(slot); return NULL; }
    fs_close(f);
    slot->data = slot->blk + doff;
    slot->frames = dlen; slot->rate = (uint32_t)rate;
    slot->ncp = dlen / AUDIO_JCP + 1;
    slot->cp = malloc(slot->ncp * sizeof(*slot->cp));
    if(!slot->cp) { jentry_free(slot); return NULL; }
    {   /* decoder checkpoints (one pass) */
        int h0 = 0, s0 = 127, h1 = 0, s1 = 127;
        for(k = 0; k < dlen; k++) {
            if(k % AUDIO_JCP == 0) { int16_t *c = slot->cp[k / AUDIO_JCP]; c[0] = (int16_t)h0; c[1] = (int16_t)s0; c[2] = (int16_t)h1; c[3] = (int16_t)s1; }
            adpcm_step(&h0, &s0, slot->data[k] >> 4);
            adpcm_step(&h1, &s1, slot->data[k] & 15);
        }
    }
    strncpy(slot->path, path, sizeof(slot->path) - 1);
    return slot;
}

/* linear play position -> file frame (loop) */
static uint32_t jpos_of(const JSlot *j, uint64_t lin)
{
    if(j->le > j->ls && lin >= j->le) return j->ls + (uint32_t)((lin - j->ls) % (j->le - j->ls));
    return lin < j->e->frames ? (uint32_t)lin : j->e->frames;
}
/* put the decoder at linear position lin (checkpoint + forward decode) */
static void jseek(JSlot *j, uint64_t lin)
{
    uint32_t p = jpos_of(j, lin), k = p / AUDIO_JCP, i;
    const int16_t *c = j->e->cp[k < j->e->ncp ? k : j->e->ncp - 1];
    j->hist[0] = c[0]; j->step[0] = c[1]; j->hist[1] = c[2]; j->step[1] = c[3];
    for(i = k * AUDIO_JCP; i < p && i < j->e->frames; i++) {
        adpcm_step(&j->hist[0], &j->step[0], j->e->data[i] >> 4);
        adpcm_step(&j->hist[1], &j->step[1], j->e->data[i] & 15);
    }
    j->pos = p; j->base = lin; j->lin = 0;
    j->pwr = j->prd = 0;
}

/* decode up to n frames into the ring (reader thread / game thread, s_lock held) */
static uint32_t jdecode(JSlot *j, uint32_t n)
{
    uint32_t room = AUDIO_JRING - (j->pwr - j->prd), k;
    const uint8_t *d = j->e->data;
    int h0 = j->hist[0], s0 = j->step[0], h1 = j->hist[1], s1 = j->step[1];
    uint32_t pos = j->pos, end = j->e->frames, ls = j->ls, le = j->le;
    if(n > room) n = room;
    for(k = 0; k < n; k++) {
        int16_t *o = &j->pcm[2 * ((j->pwr + k) % AUDIO_JRING)];
        if(pos >= end) break;
        o[0] = (int16_t)adpcm_step(&h0, &s0, d[pos] >> 4);
        o[1] = (int16_t)adpcm_step(&h1, &s1, d[pos] & 15);
        pos++;
        if(le > ls && pos >= le) pos = ls;   /* seam state at le = state at ls (aicaenc --loop) */
    }
    j->hist[0] = h0; j->step[0] = s0; j->hist[1] = h1; j->step[1] = s1;
    j->pos = pos; j->pwr += k; j->lin += k;
    return k;
}
static int jdone_decoding(const JSlot *j) { return !(j->le > j->ls) && j->pos >= j->e->frames; }

/* snd_stream callback (poll thread, s_lock held): PCM ring -> AICA (the reader
 * thread decodes; nothing heavy here) */
static void *jingle_cb(snd_stream_hnd_t hnd, int req, int *got)
{
    JSlot *j = NULL;
    int i;
    uint32_t n, have = 0, k;
    uint64_t u0 = timer_us_gettime64();
    for(i = 0; i < AUDIO_JINGLES; i++) if(s_js[i].active && s_js[i].hnd == hnd) j = &s_js[i];
    if(req > (int)sizeof(s_jbuf)) req = sizeof(s_jbuf);
    req &= ~31;
    n = (uint32_t)req / 4;
    if(j) {
        have = j->pwr - j->prd;
        if(have > n) have = n;
        for(k = 0; k < have; ) {
            uint32_t at = (j->prd + k) % AUDIO_JRING, c = AUDIO_JRING - at;
            if(c > have - k) c = have - k;
            memcpy(&s_jbuf[2 * k], &j->pcm[2 * at], c * 4);
            k += c;
        }
        j->prd += have;
        if(have < n && !jdone_decoding(j)) j->underruns++;
    }
    if(have < n) memset(&s_jbuf[2 * have], 0, (n - have) * 4);
    {
        uint32_t us = (uint32_t)(timer_us_gettime64() - u0);
        if(us > s_st.jingle_fill_max_us) s_st.jingle_fill_max_us = us;
    }
    *got = req;
    return s_jbuf;
}

static void jstream_stop(JSlot *j)
{
    if(j->hnd != SND_STREAM_INVALID) { snd_stream_stop(j->hnd); snd_stream_destroy(j->hnd); j->hnd = SND_STREAM_INVALID; }
}
static void jstream_start(JSlot *j, uint64_t now)   /* poll thread, ring prefilled */
{
    j->start_pending = 0;
    j->hnd = snd_stream_alloc(jingle_cb, AUDIO_JBUF);
    if(j->hnd == SND_STREAM_INVALID) { j->active = 0; return; }
    j->sent = -1;
    j->t_start = now;
    snd_stream_start(j->hnd, j->e->rate, 1);
    s_st.jingle_starts++;
}
/* frames the AICA has played since the last (re)start (wall clock; ~10 ms) */
static uint64_t jplayed(const JSlot *j, uint64_t now) { return j->base + (now - j->t_start) * j->e->rate / 1000; }

static volatile int s_thread_run, s_restart, s_kick;
/* load fade (KOS stream volume 0..255 = linear amplitude v/255 through the
 * KOS logs[] -> TL table); the poll thread ramps s_vol_cur to s_vol_target by
 * AUDIO_FADE_STEP per 10 ms poll (255 / 26 -> ~260 ms) */
#define AUDIO_FADE_STEP 10
static volatile int s_vol_target = 255, s_vol_cur = 255;
static int s_load_depth, s_load_frames;
static uint32_t s_prev_streamed;
const AudioStats *audio_stats(void) { return &s_st; }
static volatile uint32_t s_read_ms = 1000;  /* duration of the reader's last chunk */
static kthread_t *s_thread, *s_reader;

static int kos_vol(int v127) { return (v127 * s_vol_cur + 63) / 127; }   /* 0..255 */

static void music_destroy_streams(void)
{
    int L;
    for(L = 0; L < MAXL; L++)
        if(s_hnd[L] != SND_STREAM_INVALID) { snd_stream_stop(s_hnd[L]); snd_stream_destroy(s_hnd[L]); s_hnd[L] = SND_STREAM_INVALID; }
}
static void music_close_locked(void)   /* s_lock held */
{
    music_destroy_streams();
    s_restart = 0; s_dry = 0; s_kick = 0; s_mstop = 0;
    mutex_lock(&s_flock);
    if(s_fd != FILEHND_INVALID) { fs_close(s_fd); s_fd = FILEHND_INVALID; }
    s_wr = s_rd[0] = s_rd[1] = 0;
    mutex_unlock(&s_flock);
}

/* Layer re-sync: the two streams were keyed on several ms apart; both are still
 * inside the first half of their AICA buffers (prefilled with file position 0,
 * no refill yet: s_lock is held). One command batch keys all their channels off
 * and on again with one sync key-on, so they restart together from buffer
 * offset 0 = file position 0, where the AICA ADPCM decoder's reset state matches
 * the encoder's. The channels are checked first (keyed on, ADPCM long-stream
 * format); if they are not the predicted ones nothing is sent. */
#define AICA_CHREG0(ch) (0xa0700000u + 0x80u * (uint32_t)(ch))
static void layer_resync(const int *pc, int np)
{
    AICA_CMDSTR_CHANNEL(tmp, cmd, chan);
    uint32_t mask = 0;
    int i;
    int tries;
    for(i = 0; i < np; i++) if(pc[i] < 0 || pc[i] >= 32) return;
    /* the AICA (ARM) processes the last key-on a little after snd_stream_start returns */
    for(tries = 0; tries < 20; tries++) {
        for(i = 0, mask = 0; i < np; i++) {
            uint32_t r = g2_read_32(AICA_CHREG0(pc[i]));
            if((r & 0x4000) && ((r >> 7) & 3) == AICA_SM_ADPCM_LS) mask |= 1u << pc[i];
        }
        if(__builtin_popcount(mask) == np) break;
        thd_sleep(1);
    }
    if(__builtin_popcount(mask) != np) {
        if(s_trace) printf("strm layer re-sync: channels %d-%d not keyed on as ADPCM streams\n", pc[0], pc[np - 1]);
        return;
    }
    snd_sh4_to_aica_stop();
    for(i = 0; i < np; i++) {
        cmd->cmd = AICA_CMD_CHAN; cmd->timestamp = 0; cmd->size = AICA_CMDSTR_CHANNEL_SIZE;
        cmd->cmd_id = (uint32_t)pc[i];
        chan->cmd = AICA_CH_CMD_STOP;
        snd_sh4_to_aica(tmp, cmd->size);
    }
    cmd->cmd = AICA_CMD_CHAN; cmd->timestamp = 0; cmd->size = AICA_CMDSTR_CHANNEL_SIZE;
    cmd->cmd_id = mask;
    chan->cmd = AICA_CH_CMD_START | AICA_CH_START_SYNC;
    snd_sh4_to_aica(tmp, cmd->size);
    snd_sh4_to_aica_start();
    s_st.layer_resyncs++;
}

static void *stream_thread(void *arg)
{
    uint64_t last = 0;
    (void)arg;
    while(s_thread_run) {
        uint64_t t0 = timer_ms_gettime64(), t1, u0 = timer_us_gettime64();
        int r = 0, L, i;
        mutex_lock(&s_lock);
        if(s_hnd[0] != SND_STREAM_INVALID && s_dry) {
            /* underrun: silence now, restart from the top of the file once the ring is refilled */
            music_destroy_streams();
            mutex_lock(&s_flock);
            s_pos = 0; s_wr = s_rd[0] = s_rd[1] = 0;
            mutex_unlock(&s_flock);
            s_restart = 1;
            if(s_trace) printf("strm stopped after underrun, restarting from the top\n");
        }
        if(s_hnd[0] == SND_STREAM_INVALID && s_restart && s_wr - s_rd[0] >= (s_nl > 1 ? s_ringl / 2 : AUDIO_RESTART) && (s_kick || s_read_ms < 300)) {
            if(s_trace) printf("strm start: %d layer(s), ring %lu B, %s\n", s_nl, (unsigned long)(s_wr - s_rd[0]), s_kick ? "game loop running" : "disc idle");
            s_dry = 0; s_restart = 0;
            int pc[2 * MAXL], np = 2 * s_nl, i2;
            /* the AICA channels snd_stream_alloc will take (lowest free first, all stream
             * allocations happen in this thread): needed for the layer re-sync below */
            for(i2 = 0; i2 < np; i2++) pc[i2] = s_nl > 1 ? snd_sfx_chn_alloc() : -1;
            for(i2 = 0; i2 < np; i2++) if(pc[i2] >= 0) snd_sfx_chn_free(pc[i2]);
            /* the second layer gets a small AICA buffer: its prefill (the key-on skew, see below) is shorter */
            for(L = 0; L < s_nl; L++) s_hnd[L] = snd_stream_alloc(stream_cb, L ? AUDIO_AICA_BUF_L1 : AUDIO_AICA_BUF);
            if(s_hnd[0] == SND_STREAM_INVALID || (s_nl > 1 && s_hnd[1] == SND_STREAM_INVALID)) {
                music_destroy_streams();
            } else {
                /* layers start back to back (KOS 2.3 cannot queue the start of two stereo
                 * streams: snd_stream_start_type takes the AICA queue lock that queueing
                 * keeps held -> recursive-lock assertion); the second key-on follows the
                 * first by the second stream's prefill time (measured: AUDIO.md 14) */
                uint64_t k0 = 0, k1;
                for(L = 0; L < s_nl; L++) {
                    snd_stream_start_adpcm(s_hnd[L], s_rate, s_stereo);
                    k1 = timer_us_gettime64();
                    if(L == 0) k0 = k1; else s_st.layer_skew_us = (uint32_t)(k1 - k0);
                    s_lsent[L] = -1;   /* KOS starts at 255 */
                }
                if(s_nl > 1) layer_resync(pc, np);
                if(s_nl > 1 && s_trace) printf("strm layers keyed on %lu us apart, re-synced: %s\n", (unsigned long)s_st.layer_skew_us,
                                               s_st.layer_resyncs ? "yes" : "NO (channels not found)");
                s_st.starts++;
            }
        }
        /* load fade (audio_load_begin / audio_load_end) */
        if(s_vol_cur != s_vol_target) {
            int v = s_vol_cur, t = s_vol_target;
            v = v < t ? (v + AUDIO_FADE_STEP > t ? t : v + AUDIO_FADE_STEP) : (v - AUDIO_FADE_STEP < t ? t : v - AUDIO_FADE_STEP);
            s_vol_cur = v;
        }
        /* music layer volumes, fade-stop */
        if(s_mstop) {
            int done = 1;
            for(L = 0; L < s_nl; L++) if(!fader_done(&s_lfader[L], t0)) done = 0;
            if(done) { music_close_locked(); if(s_trace) printf("strm fade-stop done\n"); }
        }
        for(L = 0; L < s_nl; L++) if(s_hnd[L] != SND_STREAM_INVALID) {
            int v = kos_vol(fader_get(&s_lfader[L], t0));
            if(v != s_lsent[L]) { snd_stream_volume(s_hnd[L], v); s_lsent[L] = v; }
        }
        for(L = 0; L < s_nl; L++) if(s_hnd[L] != SND_STREAM_INVALID) { int rr = snd_stream_poll(s_hnd[L]); if(rr < 0) r = rr; }
        /* jingles */
        for(i = 0; i < AUDIO_JINGLES; i++) {
            JSlot *j = &s_js[i];
            if(!j->active || j->paused) continue;
            if(j->start_pending && (j->pwr - j->prd >= AUDIO_JPREFILL || jdone_decoding(j))) jstream_start(j, t0);
            if(j->hnd == SND_STREAM_INVALID) continue;
            if(j->stop_at_zero && fader_done(&j->f, t0) && fader_get(&j->f, t0) == 0) { jstream_stop(j); j->active = 0; continue; }
            if(j->le <= j->ls && jplayed(j, t0) >= j->e->frames + (uint64_t)j->e->rate / 20) {   /* played to the end (+50 ms) */
                jstream_stop(j); j->active = 0; j->ended = 1;
                if(s_trace) printf("jingle %d ended (%s)\n", i, j->e->path);
                continue;
            }
            {
                int v = kos_vol(fader_get(&j->f, t0));
                if(v != j->sent) { snd_stream_volume(j->hnd, v); j->sent = v; }
            }
            snd_stream_poll(j->hnd);
        }
        mutex_unlock(&s_lock);
        t1 = timer_ms_gettime64();
        if(s_streamed != s_prev_streamed) {   /* a refill happened: keep its CPU time (stats) */
            uint32_t us = (uint32_t)(timer_us_gettime64() - u0);
            if(us > s_st.fill_max_us) s_st.fill_max_us = us;
            s_st.fills++;
            s_prev_streamed = s_streamed;
        }
        if(last && t0 - last > s_st.poll_gap_max_ms) s_st.poll_gap_max_ms = (uint32_t)(t0 - last);
        if(r < 0) s_st.poll_errors++;
        last = t1;
        thd_sleep(10);
    }
    return NULL;
}

static void *reader_thread(void *arg)
{
    uint32_t next_log = 0;
    (void)arg;
    while(s_thread_run) {
        uint32_t got = 0, dec = 0;
        uint64_t t0 = timer_ms_gettime64(), t1;
        int i;
        /* RAM jingles: ADPCM -> PCM ring, in the game loop's idle time (low priority) */
        for(i = 0; i < AUDIO_JINGLES; i++) {
            uint32_t d;
            do {
                mutex_lock(&s_lock);
                d = s_js[i].active && !s_js[i].paused ? jdecode(&s_js[i], AUDIO_JCHUNK) : 0;
                mutex_unlock(&s_lock);
                dec += d;
            } while(d == AUDIO_JCHUNK);
        }
        mutex_lock(&s_flock);
        if(s_ringl - ring_used() >= AUDIO_CHUNK / (uint32_t)s_nl) got = fill_ring(AUDIO_CHUNK / (uint32_t)s_nl);
        mutex_unlock(&s_flock);
        t1 = timer_ms_gettime64();
        if(got) {
            s_read_ms = (uint32_t)(t1 - t0);
            s_st.reads++; s_st.read_bytes += got * (uint32_t)s_nl;
            if(s_read_ms > s_st.read_max_ms) s_st.read_max_ms = s_read_ms;
            s_st.last_read_ms = s_read_ms;
        }
        s_st.ring_low = s_low;
        if(s_trace && (uint32_t)(t1 / 1000) >= next_log) {   /* one summary line per 10 s (serial output is slow) */
            printf("strm t%lu: %lu reads (%lu B, max %lu ms), %lu fills (max %lu us), poll gap max %lu ms, ring %lu B (low %lu), underruns %lu, layers %d, jingle fill max %lu us\n",
                   (unsigned long)t1, (unsigned long)s_st.reads, (unsigned long)s_st.read_bytes, (unsigned long)s_st.read_max_ms,
                   (unsigned long)s_st.fills, (unsigned long)s_st.fill_max_us, (unsigned long)s_st.poll_gap_max_ms,
                   (unsigned long)(s_wr - s_rd[0]), (unsigned long)s_low, (unsigned long)s_underruns, s_nl,
                   (unsigned long)s_st.jingle_fill_max_us);
            next_log = (uint32_t)(t1 / 1000) + 10;
        }
        if(!got) thd_sleep(dec ? 5 : 20);
    }
    return NULL;
}

void audio_init(void)
{
    int i;
    snd_stream_init();
    for(i = 0; i < AUDIO_JINGLES; i++) s_js[i].hnd = SND_STREAM_INVALID;
    s_lfader[0].to = 127;
    s_thread_run = 1;
    s_thread = thd_create(0, stream_thread, NULL);
    s_reader = thd_create(0, reader_thread, NULL);
    if(s_reader) thd_set_prio(s_reader, AUDIO_READER_PRIO);
}

uint32_t audio_slot_gen(int slot) { return slot >= 0 && slot < 3 ? s_gen[slot] : 0; }

int audio_music_play_layers(const char *path, uint32_t loop_start, uint32_t loop_end, int vol0, int vol1)
{
    int rate = 0, ch = 0, nl = 1;
    uint32_t doff = 0, dlen = 0, block = 0;
    uint8_t h[40];
    uint64_t now = timer_ms_gettime64();
    file_t f;
    mutex_lock(&s_lock);
    music_close_locked();
    s_gen[0]++;
    mutex_unlock(&s_lock);
    f = fs_open(path, O_RDONLY);
    if(f == FILEHND_INVALID) return -1;
    if(fs_read(f, h, sizeof(h)) == (ssize_t)sizeof(h) && !memcmp(h, "SRLY", 4)) {
        /* musictab.py: "SRLY", version, layers, rate, frames, loop start, loop end, block, data offset, blocks */
        uint32_t v[9];
        memcpy(v, h + 4, sizeof(v));
        if(v[0] != 1 || v[1] < 1 || v[1] > MAXL || v[6] == 0 || v[6] * v[1] > AUDIO_CHUNK || (v[7] & 2047)) { fs_close(f); return -1; }
        nl = (int)v[1]; rate = (int)v[2]; ch = 2; dlen = v[3]; block = v[6]; doff = v[7];
        loop_start = v[4]; loop_end = v[5];
    } else {
        fs_seek(f, 0, SEEK_SET);
        if(read_wav_header(f, &rate, &ch, &doff, &dlen)) { fs_close(f); return -1; }
    }
    {
        file_t t = fs_open("/cd/debug/sndtrace.txt", O_RDONLY);
        s_trace = t != FILEHND_INVALID;
        if(t != FILEHND_INVALID) fs_close(t);
    }
    mutex_lock(&s_lock);
    mutex_lock(&s_flock);
    s_fd = f; s_rate = rate; s_stereo = ch == 2; s_data_off = doff; s_data_len = dlen;
    s_nl = nl; s_lyr_block = block; s_ringl = AUDIO_RING_BYTES / (uint32_t)nl;
    /* loop points in sample frames; one byte holds 2 mono samples or 1 stereo frame
     * (wav2adpcm -i nibble interleave) */
    s_loop_start_b = s_stereo ? loop_start : loop_start / 2;
    s_loop_end_b = s_stereo ? loop_end : loop_end / 2;
    s_pos = 0; s_wr = s_rd[0] = s_rd[1] = 0; s_low = s_ringl; s_dry = 0;
    s_read_ms = 1000; s_kick = 0;
    if(!s_lyr_block) fs_seek(s_fd, s_data_off, SEEK_SET);
    mutex_unlock(&s_flock);
    memset(s_lfader, 0, sizeof(s_lfader));
    s_lfader[0].to = s_lfader[0].from = vol0 < 0 ? 0 : vol0 > 127 ? 127 : vol0;
    s_lfader[1].to = s_lfader[1].from = vol1 < 0 ? 0 : vol1 > 127 ? 127 : vol1;
    (void)now;
    s_mstop = 0;
    s_restart = 1;                            /* the poll thread starts the stream (see top) */
    mutex_unlock(&s_lock);
    if(s_trace) printf("music %s: %d layer(s), %lu frames, loop %lu-%lu, vol %d/%d\n", path, nl, (unsigned long)dlen,
                       (unsigned long)loop_start, (unsigned long)loop_end, vol0, vol1);
    return 0;
}

int audio_music_play(const char *path, uint32_t loop_start, uint32_t loop_end)
{
    int i;
    /* Sound_PlaySeq on gBGMSoundHandle replaces whatever played on it (a 1-up jingle too) */
    mutex_lock(&s_lock);
    for(i = 0; i < AUDIO_JINGLES; i++) { jstream_stop(&s_js[i]); s_js[i].active = 0; s_gen[1 + i]++; }
    mutex_unlock(&s_lock);
    return audio_music_play_layers(path, loop_start, loop_end, 127, 0);
}

void audio_music_layer_volume(int layer, int vol, int frames)
{
    if(layer < 0 || layer >= MAXL) return;
    mutex_lock(&s_lock);
    fader_set(&s_lfader[layer], vol, frames, timer_ms_gettime64());
    mutex_unlock(&s_lock);
}

void audio_music_fade_stop(int frames)
{
    int L;
    uint64_t now = timer_ms_gettime64();
    mutex_lock(&s_lock);
    s_gen[0]++;
    if(frames <= 0 || s_hnd[0] == SND_STREAM_INVALID) music_close_locked();
    else {
        for(L = 0; L < MAXL; L++) fader_set(&s_lfader[L], 0, frames, now);
        s_mstop = 1;
        s_restart = 0;     /* still buffering: never start */
    }
    mutex_unlock(&s_lock);
}

int audio_music_active(void) { return s_fd != FILEHND_INVALID; }

void audio_music_stop(void)
{
    int i;
    mutex_lock(&s_lock);
    music_close_locked();
    s_gen[0]++;
    for(i = 0; i < AUDIO_JINGLES; i++) { jstream_stop(&s_js[i]); s_js[i].active = 0; s_gen[1 + i]++; }
    mutex_unlock(&s_lock);
}

/* ------------------------------------------------------------ jingle API */
int audio_jingle_preload(const char *path)
{
    uint64_t t0 = timer_ms_gettime64();
    int hit = jcache_find(path) != NULL;
    JEntry *e = jcache_load(path);   /* game thread; no s_lock during the disc read */
    if(s_trace) printf("jingle preload %s: %s, %lu frames, %lu ms\n", path, !e ? "FAILED" : hit ? "cached" : "read",
                       e ? (unsigned long)e->frames : 0ul, (unsigned long)(timer_ms_gettime64() - t0));
    return e ? 0 : -1;
}

void audio_jingle_flush(void)
{
    int i;
    mutex_lock(&s_lock);
    for(i = 0; i < AUDIO_JINGLES; i++) { jstream_stop(&s_js[i]); s_js[i].active = 0; s_gen[1 + i]++; }
    for(i = 0; i < AUDIO_JCACHE; i++) if(s_jc[i].blk) jentry_free(&s_jc[i]);
    mutex_unlock(&s_lock);
}

int audio_jingle_play(int slot, const char *path, uint32_t loop_start, uint32_t loop_end, int vol)
{
    JSlot *j;
    JEntry *e;
    uint64_t now = timer_ms_gettime64();
    if(slot < 0 || slot >= AUDIO_JINGLES) return -1;
    j = &s_js[slot];
    mutex_lock(&s_lock);
    jstream_stop(j); j->active = 0;
    s_gen[1 + slot]++;
    mutex_unlock(&s_lock);
    e = jcache_load(path);   /* cache: game thread only; blocking read outside s_lock (the poll thread keeps the music fed) */
    mutex_lock(&s_lock);
    if(!e) { mutex_unlock(&s_lock); return -1; }
    j->e = e;
    j->ls = loop_start; j->le = loop_end <= e->frames ? loop_end : e->frames;
    if(j->le <= j->ls) j->ls = j->le = 0;
    j->paused = 0; j->ended = 0; j->stop_at_zero = 0;
    memset(&j->f, 0, sizeof(j->f));
    j->f.to = j->f.from = vol < 0 ? 0 : vol > 127 ? 127 : vol;
    jseek(j, 0);
    j->active = 1;
    j->start_pending = 1;      /* the reader decodes, the poll thread starts the stream */
    (void)now;
    mutex_unlock(&s_lock);
    if(s_trace) printf("jingle %d play %s loop %lu-%lu vol %d\n", slot, path, (unsigned long)loop_start, (unsigned long)loop_end, vol);
    return j->active ? 0 : -1;
}

void audio_jingle_stop(int slot, int frames)
{
    JSlot *j;
    if(slot < 0 || slot >= AUDIO_JINGLES) return;
    j = &s_js[slot];
    mutex_lock(&s_lock);
    s_gen[1 + slot]++;
    if(j->active) {
        if(frames <= 0 || j->paused || j->hnd == SND_STREAM_INVALID) { jstream_stop(j); j->active = 0; }
        else { fader_set(&j->f, 0, frames, timer_ms_gettime64()); j->stop_at_zero = 1; }
    }
    mutex_unlock(&s_lock);
}

void audio_jingle_pause(int slot, int on)
{
    JSlot *j;
    uint64_t now = timer_ms_gettime64();
    if(slot < 0 || slot >= AUDIO_JINGLES) return;
    j = &s_js[slot];
    mutex_lock(&s_lock);
    if(j->active && on && !j->paused) {
        uint64_t p = j->hnd != SND_STREAM_INVALID ? jplayed(j, now) : j->base;   /* not started yet: still at base */
        int v = fader_get(&j->f, now);
        jstream_stop(j);
        j->paused = 1;
        j->f.from = j->f.to = v; j->f.dur = 0;   /* a fade in progress holds its value while paused */
        j->base = p;            /* resume here */
        if(s_trace) printf("jingle %d paused at frame %llu\n", slot, (unsigned long long)p);
    } else if(j->active && !on && j->paused) {
        jseek(j, j->base);
        j->paused = 0;
        j->start_pending = 1;
        if(s_trace) printf("jingle %d resumed at frame %llu\n", slot, (unsigned long long)j->base);
    }
    mutex_unlock(&s_lock);
}

void audio_jingle_volume(int slot, int vol, int frames)
{
    if(slot < 0 || slot >= AUDIO_JINGLES) return;
    mutex_lock(&s_lock);
    fader_set(&s_js[slot].f, vol, frames, timer_ms_gettime64());
    mutex_unlock(&s_lock);
}

int audio_jingle_active(int slot) { return slot >= 0 && slot < AUDIO_JINGLES && s_js[slot].active; }

void audio_update(void)
{
    /* polling and disc reads happen on the stream / reader threads; a call
     * here tells them the game loop runs (no stage load in progress) */
    s_kick = 1;
    /* safety: a load bracket left open while the game loop runs again for
     * 2 s is closed here, so the music is never left muted */
    if(s_load_depth && ++s_load_frames > 120) {
        if(s_trace) printf("audio_load_end missing: restoring the music volume\n");
        s_load_depth = 0; s_vol_target = 255;
    }
}

/* Loading bracket (call from the game thread):
 *   audio_load_begin()  fades the music (stream volume, ~260 ms, poll thread)
 *                       and all effects (here, sound_update() kept running at
 *                       ~60 Hz meanwhile) to silence, then stops the effects
 *                       and returns; the music stream keeps playing muted (the
 *                       poll / reader threads keep the AICA ring serviced, the
 *                       track position advances as the original's BGM would),
 *                       a new audio_music_play() inside the bracket starts muted.
 *   audio_load_end()    fades the music back to full over ~260 ms (non-blocking).
 * Nested calls count; only the outermost pair fades. Levels outside the
 * bracket are unchanged (no change to the mix). */
void audio_load_begin(void)
{
    int i;
    const int steps = 16;   /* 16 x 16 ms = 256 ms, same length as the music ramp */
    if(s_load_depth++) return;
    s_load_frames = 0;
    s_vol_target = 0;
    for(i = 1; i <= steps; i++) {
        sound_set_fade((uint32_t)(65536 * (steps - i) / steps));
        sound_update();
        thd_sleep(16);
    }
    for(i = 0; i < 40 && s_vol_cur != 0; i++) thd_sleep(10);   /* music ramp done (poll thread) */
    sound_stop_all();
    sound_update();          /* pushes the key-offs */
    sound_set_fade(65536);   /* effects started after the load play at full level */
}

void audio_load_end(void)
{
    if(s_load_depth == 0) return;
    if(--s_load_depth == 0) s_vol_target = 255;
}

int audio_sfx_load(const char *path)
{
    sfxhnd_t h = snd_sfx_load(path);
    return h == SFXHND_INVALID ? -1 : (int)h;
}

void audio_sfx_play(int h, int vol, int pan)
{
    if(h >= 0) snd_sfx_play((sfxhnd_t)h, vol, pan);
}
