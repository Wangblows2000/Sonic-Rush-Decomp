#include "audio.h"
#include <kos.h>
#include <dc/sound/sound.h>
#include <dc/sound/stream.h>
#include <dc/sound/sfxmgr.h>
#include <string.h>
#include <stdio.h>
#include <kos/thread.h>

/* Music: snd_stream in ADPCM mode, fed from a file. AICA ADPCM is 4-bit,
 * 2 samples per byte; WAV data chunk is read directly (no SH-4 decoding). */
static snd_stream_hnd_t s_hnd = SND_STREAM_INVALID;
static file_t s_fd = FILEHND_INVALID;
static uint32_t s_data_off, s_data_len, s_pos, s_loop_start_b, s_loop_end_b;
static int s_stereo, s_rate;
static volatile uint32_t s_streamed, s_loops;
uint32_t audio_stream_bytes(void) { return s_streamed; }
uint32_t audio_stream_loops(void) { return s_loops; }
static uint8_t s_buf[SND_STREAM_BUFFER_MAX_ADPCM * 2] __attribute__((aligned(32)));

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

static void *stream_cb(snd_stream_hnd_t hnd, int req, int *got)
{
    int done = 0;
    uint32_t end = s_loop_end_b && s_loop_end_b <= s_data_len ? s_loop_end_b : s_data_len;
    (void)hnd;
    if(req > (int)sizeof(s_buf)) req = sizeof(s_buf);
    req &= ~31;
    while(done < req) {
        int n = req - done, r;
        if(s_pos >= end) {
            s_pos = s_loop_start_b < end ? s_loop_start_b : 0;
            s_loops++;
            fs_seek(s_fd, s_data_off + s_pos, SEEK_SET);   /* seek only at the loop point */
        }
        if((uint32_t)n > end - s_pos) n = end - s_pos;
        r = fs_read(s_fd, s_buf + done, n);
        if(r <= 0) break;
        done += r; s_pos += r;
    }
    s_streamed += done;
    *got = done;
    return s_buf;
}

/* Streaming runs on its own thread so disc reads never stall the 60 Hz frame. */
static volatile int s_thread_run;
static kthread_t *s_thread;
static mutex_t s_lock = MUTEX_INITIALIZER;

static void *stream_thread(void *arg)
{
    (void)arg;
    while(s_thread_run) {
        mutex_lock(&s_lock);
        if(s_hnd != SND_STREAM_INVALID) snd_stream_poll(s_hnd);
        mutex_unlock(&s_lock);
        thd_sleep(10);
    }
    return NULL;
}

void audio_init(void)
{
    snd_stream_init();
    s_thread_run = 1;
    s_thread = thd_create(0, stream_thread, NULL);
}

int audio_music_play(const char *path, uint32_t loop_start, uint32_t loop_end)
{
    audio_music_stop();
    mutex_lock(&s_lock);
    s_fd = fs_open(path, O_RDONLY);
    if(s_fd == FILEHND_INVALID) { mutex_unlock(&s_lock); return -1; }
    if(read_wav_header(s_fd, &s_rate, &s_stereo, &s_data_off, &s_data_len)) { fs_close(s_fd); s_fd = FILEHND_INVALID; mutex_unlock(&s_lock); return -1; }
    s_stereo = s_stereo == 2;
    /* 4-bit samples; stereo ADPCM data is stored left block / right block by KOS
     * wav2adpcm per buffer, so loop points are kept on byte boundaries. */
    s_loop_start_b = loop_start / 2 * (s_stereo ? 2 : 1);
    s_loop_end_b = loop_end / 2 * (s_stereo ? 2 : 1);
    s_pos = 0;
    fs_seek(s_fd, s_data_off, SEEK_SET);
    s_hnd = snd_stream_alloc(stream_cb, SND_STREAM_BUFFER_MAX_ADPCM);
    if(s_hnd == SND_STREAM_INVALID) { mutex_unlock(&s_lock); return -1; }
    snd_stream_start_adpcm(s_hnd, s_rate, s_stereo);
    mutex_unlock(&s_lock);
    return 0;
}

void audio_music_stop(void)
{
    mutex_lock(&s_lock);
    if(s_hnd != SND_STREAM_INVALID) { snd_stream_stop(s_hnd); snd_stream_destroy(s_hnd); s_hnd = SND_STREAM_INVALID; }
    if(s_fd != FILEHND_INVALID) { fs_close(s_fd); s_fd = FILEHND_INVALID; }
    mutex_unlock(&s_lock);
}

void audio_update(void)
{
    /* polling happens on the stream thread */
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
