#include "audio.h"
#include <kos.h>
#include <dc/sound/sound.h>
#include <dc/sound/stream.h>
#include <dc/sound/sfxmgr.h>
#include <string.h>
#include <stdio.h>
#include <kos/thread.h>

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
 * counted (audio_stream_underruns). */
#define AUDIO_RING_BYTES (384 * 1024)   /* 12.0 s of stereo ADPCM at 32728 Hz */
#define AUDIO_CHUNK (64 * 1024)         /* reader request size */
#define AUDIO_RESTART (128 * 1024)
#ifndef AUDIO_READER_PRIO
#define AUDIO_READER_PRIO (PRIO_DEFAULT + 2)  /* below the game loop: reads run in its idle time */
#endif      /* ring level needed to restart after an underrun */

static snd_stream_hnd_t s_hnd = SND_STREAM_INVALID;
static file_t s_fd = FILEHND_INVALID;
static uint32_t s_data_off, s_data_len, s_pos, s_loop_start_b, s_loop_end_b;
static int s_stereo, s_rate;
static volatile uint32_t s_streamed, s_loops, s_underruns;
static uint8_t s_ring[AUDIO_RING_BYTES] __attribute__((aligned(32)));
static volatile uint32_t s_wr, s_rd;    /* total bytes written to / consumed from the ring */
static uint8_t s_buf[SND_STREAM_BUFFER_MAX_ADPCM * 2] __attribute__((aligned(32)));
/* debug trace (serial), on when /cd/debug/sndtrace.txt exists: poll gaps or
 * polls over 100 ms, poll errors, short reads, underruns, ring low-water mark */
static int s_trace;
static uint32_t s_low = AUDIO_RING_BYTES;
static volatile int s_dry;              /* underrun seen: poll thread stops the stream */
uint32_t audio_stream_bytes(void) { return s_streamed; }
uint32_t audio_stream_loops(void) { return s_loops; }
uint32_t audio_stream_underruns(void) { return s_underruns; }

static mutex_t s_lock = MUTEX_INITIALIZER;   /* stream handle (poll thread / API) */
static mutex_t s_flock = MUTEX_INITIALIZER;  /* file + ring writer state (reader thread / API) */

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

/* disc -> ring, at most `max` bytes; s_flock held. Loops the file at
 * loop_end -> loop_start (whole file if no loop). Returns bytes read. */
static uint32_t fill_ring(uint32_t max)
{
    uint32_t done = 0;
    uint32_t end = s_loop_end_b && s_loop_end_b <= s_data_len ? s_loop_end_b : s_data_len;
    while(s_fd != FILEHND_INVALID && done < max) {
        uint32_t used = s_wr - s_rd, room = AUDIO_RING_BYTES - used, at = s_wr % AUDIO_RING_BYTES, n;
        int r;
        if(room == 0) break;
        if(s_pos >= end) {
            s_pos = s_loop_start_b < end ? s_loop_start_b : 0;
            s_loops++;
            fs_seek(s_fd, s_data_off + s_pos, SEEK_SET);   /* seek only at the loop point */
        }
        n = max - done;
        if(n > AUDIO_CHUNK) n = AUDIO_CHUNK;
        if(n > room) n = room;
        if(n > AUDIO_RING_BYTES - at) n = AUDIO_RING_BYTES - at;
        if(n > end - s_pos) n = end - s_pos;
        r = fs_read(s_fd, s_ring + at, n);
        if(s_trace && r != (int)n) printf("strm short read %d of %lu at %lu\n", r, (unsigned long)n, (unsigned long)s_pos);
        if(r <= 0) break;
        s_wr += (uint32_t)r; s_pos += (uint32_t)r; done += (uint32_t)r;
    }
    return done;
}

/* snd_stream callback (poll thread, s_lock held): ring -> AICA, never blocks */
static void *stream_cb(snd_stream_hnd_t hnd, int req, int *got)
{
    uint32_t avail = s_wr - s_rd, n, at, first;
    (void)hnd;
    if(req > (int)sizeof(s_buf)) req = sizeof(s_buf);
    req &= ~31;
    n = (uint32_t)req < avail ? (uint32_t)req : avail & ~31u;
    if(n < (uint32_t)req) {
        s_underruns++; s_dry = 1;
        if(s_trace) printf("strm underrun: %lu of %d bytes in ring\n", (unsigned long)avail, req);
    }
    at = s_rd % AUDIO_RING_BYTES;
    first = AUDIO_RING_BYTES - at < n ? AUDIO_RING_BYTES - at : n;
    memcpy(s_buf, s_ring + at, first);
    if(n > first) memcpy(s_buf + first, s_ring, n - first);
    s_rd += n;
    if(s_wr - s_rd < s_low) s_low = s_wr - s_rd;
    s_streamed += n;
    *got = (int)n;
    return s_buf;
}

static volatile int s_thread_run, s_restart, s_kick;
static volatile uint32_t s_read_ms = 1000;  /* duration of the reader's last chunk */
static kthread_t *s_thread, *s_reader;

static void *stream_thread(void *arg)
{
    uint64_t last = 0;
    (void)arg;
    while(s_thread_run) {
        uint64_t t0 = timer_ms_gettime64(), t1;
        int r = 0;
        mutex_lock(&s_lock);
        if(s_hnd != SND_STREAM_INVALID && s_dry) {
            /* underrun: silence now, restart from the top of the file once the ring is refilled */
            snd_stream_stop(s_hnd); snd_stream_destroy(s_hnd); s_hnd = SND_STREAM_INVALID;
            mutex_lock(&s_flock);
            s_pos = 0; s_wr = s_rd = 0;
            if(s_fd != FILEHND_INVALID) fs_seek(s_fd, s_data_off, SEEK_SET);
            mutex_unlock(&s_flock);
            s_restart = 1;
            if(s_trace) printf("strm stopped after underrun, restarting from the top\n");
        }
        if(s_hnd == SND_STREAM_INVALID && s_restart && s_wr - s_rd >= AUDIO_RESTART && (s_kick || s_read_ms < 300)) {
            if(s_trace) printf("strm start: ring %lu B, %s\n", (unsigned long)(s_wr - s_rd), s_kick ? "game loop running" : "disc idle");
            s_dry = 0; s_restart = 0;
            s_hnd = snd_stream_alloc(stream_cb, SND_STREAM_BUFFER_MAX_ADPCM);
            if(s_hnd != SND_STREAM_INVALID) snd_stream_start_adpcm(s_hnd, s_rate, s_stereo);
        }
        if(s_hnd != SND_STREAM_INVALID) r = snd_stream_poll(s_hnd);
        mutex_unlock(&s_lock);
        t1 = timer_ms_gettime64();
        if(s_trace && last && (t0 - last > 20 || t1 - t0 > 20 || r < 0))
            printf("strm t%lu gap %lu ms poll %lu ms r %d pos %lu\n", (unsigned long)t0, (unsigned long)(t0 - last),
                   (unsigned long)(t1 - t0), r, (unsigned long)s_streamed);
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
        uint32_t got = 0;
        uint64_t t0 = timer_ms_gettime64(), t1;
        mutex_lock(&s_flock);
        if(AUDIO_RING_BYTES - (s_wr - s_rd) >= AUDIO_CHUNK) got = fill_ring(AUDIO_CHUNK);
        mutex_unlock(&s_flock);
        t1 = timer_ms_gettime64();
        if(got) s_read_ms = (uint32_t)(t1 - t0);
        if(s_trace && got) printf("strm rd t%lu %lu B %lu ms (end t%lu)\n", (unsigned long)t0, (unsigned long)got, (unsigned long)(t1 - t0), (unsigned long)t1);
        if(s_trace && (t1 - t0 > 500 || (uint32_t)(t1 / 1000) >= next_log)) {
            printf("strm reader t%lu read %lu B in %lu ms, ring %lu B (low %lu), underruns %lu\n", (unsigned long)t0,
                   (unsigned long)got, (unsigned long)(t1 - t0), (unsigned long)(s_wr - s_rd), (unsigned long)s_low,
                   (unsigned long)s_underruns);
            next_log = (uint32_t)(t1 / 1000) + 10;
        }
        if(!got) thd_sleep(20);
    }
    return NULL;
}

void audio_init(void)
{
    snd_stream_init();
    s_thread_run = 1;
    s_thread = thd_create(0, stream_thread, NULL);
    s_reader = thd_create(0, reader_thread, NULL);
    if(s_reader) thd_set_prio(s_reader, AUDIO_READER_PRIO);
}

int audio_music_play(const char *path, uint32_t loop_start, uint32_t loop_end)
{
    int rate = 0, ch = 0;
    uint32_t doff = 0, dlen = 0;
    file_t f;
    audio_music_stop();
    f = fs_open(path, O_RDONLY);
    if(f == FILEHND_INVALID) return -1;
    if(read_wav_header(f, &rate, &ch, &doff, &dlen)) { fs_close(f); return -1; }
    {
        file_t t = fs_open("/cd/debug/sndtrace.txt", O_RDONLY);
        s_trace = t != FILEHND_INVALID;
        if(t != FILEHND_INVALID) fs_close(t);
    }
    mutex_lock(&s_flock);
    s_fd = f; s_rate = rate; s_stereo = ch == 2; s_data_off = doff; s_data_len = dlen;
    /* loop points in sample frames; one byte holds 2 mono samples or 1 stereo frame
     * (wav2adpcm -i nibble interleave) */
    s_loop_start_b = s_stereo ? loop_start : loop_start / 2;
    s_loop_end_b = s_stereo ? loop_end : loop_end / 2;
    s_pos = 0; s_wr = s_rd = 0; s_low = AUDIO_RING_BYTES; s_dry = 0;
    s_read_ms = 1000; s_kick = 0;
    fs_seek(s_fd, s_data_off, SEEK_SET);
    mutex_unlock(&s_flock);
    mutex_lock(&s_lock);
    s_restart = 1;                            /* the poll thread starts the stream (see top) */
    mutex_unlock(&s_lock);
    return 0;
}

void audio_music_stop(void)
{
    mutex_lock(&s_lock);
    if(s_hnd != SND_STREAM_INVALID) { snd_stream_stop(s_hnd); snd_stream_destroy(s_hnd); s_hnd = SND_STREAM_INVALID; }
    s_restart = 0; s_dry = 0; s_kick = 0;
    mutex_unlock(&s_lock);
    mutex_lock(&s_flock);
    if(s_fd != FILEHND_INVALID) { fs_close(s_fd); s_fd = FILEHND_INVALID; }
    s_wr = s_rd = 0;
    mutex_unlock(&s_flock);
}

void audio_update(void)
{
    /* polling and disc reads happen on the stream / reader threads; a call
     * here tells them the game loop runs (no stage load in progress) */
    s_kick = 1;
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
