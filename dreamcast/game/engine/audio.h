/* Native AICA audio: streamed music (ADPCM from disc) + one-shot SFX in sound RAM. */
#ifndef SRDC_AUDIO_H
#define SRDC_AUDIO_H
#include <stdint.h>
void audio_init(void);
/* Play an AICA-ADPCM WAV (sdat2dc.py output) from disc as a looping stream.
 * loop_start/loop_end are in sample frames (0,0 = loop whole file). Returns 0
 * on success. Non-blocking: a reader thread fills a 384 KB RAM ring and the
 * stream starts once the ring holds 128 KB and either audio_update() has been
 * called since (game loop running) or the disc is idle (see audio.c). */
int  audio_music_play(const char *path, uint32_t loop_start, uint32_t loop_end);
void audio_music_stop(void);
void audio_load_begin(void);        /* fade music + effects out before a heavy load (blocks ~0.26 s) */
void audio_load_end(void);          /* fade the music back in after it (non-blocking) */
void audio_update(void);            /* call once per game-loop frame (marks "loading over") */
int  audio_sfx_load(const char *path);  /* returns handle or -1 */
void audio_sfx_play(int h, int vol, int pan);
uint32_t audio_stream_bytes(void);  /* bytes handed to the AICA stream so far */
uint32_t audio_stream_loops(void);
uint32_t audio_stream_underruns(void);
/* stream statistics since boot (debug / profiling) */
typedef struct {
    uint32_t reads, read_bytes, read_max_ms, last_read_ms;   /* reader thread disc reads */
    uint32_t fills, fill_max_us;                             /* AICA refills (poll thread CPU time) */
    uint32_t poll_gap_max_ms, poll_errors;
} AudioStats;
const AudioStats *audio_stats(void); /* refills the RAM ring could not cover (0 = clean stream) */
#endif
