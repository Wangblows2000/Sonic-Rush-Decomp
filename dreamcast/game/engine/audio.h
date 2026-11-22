/* Native AICA audio: streamed music (ADPCM from disc), RAM jingles, one-shot SFX in sound RAM. */
#ifndef SRDC_AUDIO_H
#define SRDC_AUDIO_H
#include <stdint.h>
void audio_init(void);

/* ---- disc music (one AICA-ADPCM stream per layer) ----
 * Play an AICA-ADPCM WAV (sdat2dc.py output) or a layer file (.lyr, musictab.py:
 * two tracks interleaved in 32 KB blocks, played as two streams started together)
 * from disc as a looping stream. loop_start/loop_end are in sample frames (0,0 =
 * loop whole file; a .lyr carries its own loop). Returns 0 on success.
 * Non-blocking: a reader thread fills a 384 KB RAM ring and the stream starts
 * once the ring holds 128 KB and either audio_update() has been called since
 * (game loop running) or the disc is idle (see audio.c).
 * audio_music_play: layer 0 at full volume (layer 1 of a .lyr muted). */
int  audio_music_play(const char *path, uint32_t loop_start, uint32_t loop_end);
/* same with the initial layer volumes (0..127, NNS player volume units) */
int  audio_music_play_layers(const char *path, uint32_t loop_start, uint32_t loop_end, int vol0, int vol1);
/* NNS_SndPlayerMoveVolume on one layer: linear move to vol (0..127) in `frames` 60 Hz frames */
void audio_music_layer_volume(int layer, int vol, int frames);
/* NNS_SndPlayerStopSeq(handle, frames): fade every layer to 0 over `frames`, then stop */
void audio_music_fade_stop(int frames);
int  audio_music_active(void);      /* a disc track is open (playing, buffering or fading) */
void audio_music_stop(void);        /* stops the disc music AND both jingle slots */

/* ---- RAM jingles (two slots, PCM16 streams decoded on the SH-4 from RAM-resident
 * AICA-ADPCM WAVs): exact pause / resume and position, no disc access while playing.
 * audio_jingle_preload reads a file into the cache (call inside a load bracket);
 * play loads it first if it is not cached (blocking read). ---- */
enum { AUDIO_JINGLE_0 = 0, AUDIO_JINGLE_1 = 1, AUDIO_JINGLES = 2 };
int  audio_jingle_preload(const char *path);
void audio_jingle_flush(void);      /* free the cache (stops both slots) */
int  audio_jingle_play(int slot, const char *path, uint32_t loop_start, uint32_t loop_end, int vol);
void audio_jingle_stop(int slot, int frames);   /* fade out over frames (0 = now), then stop */
void audio_jingle_pause(int slot, int on);      /* NNS_SndPlayerPause: keeps the position */
void audio_jingle_volume(int slot, int vol, int frames);
int  audio_jingle_active(int slot); /* playing or paused (not ended / stopped) */
/* start/stop counter of a slot (0..2: music, jingle 0, jingle 1): changes on every
 * play / stop through this API, not when a sound ends by itself */
uint32_t audio_slot_gen(int slot);

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
    uint32_t starts, ring_low;                               /* music stream starts, ring low-water (bytes) */
    uint32_t jingle_starts, jingle_fill_max_us;              /* RAM jingle streams */
    uint32_t layer_skew_us;                                  /* key-on of layer 1 after layer 0 (last start) */
    uint32_t layer_resyncs;                                  /* layer starts re-synchronised (one sync key-on) */
} AudioStats;
const AudioStats *audio_stats(void); /* refills the RAM ring could not cover (0 = clean stream) */
#endif
