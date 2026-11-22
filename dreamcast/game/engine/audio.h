/* Native AICA audio: streamed music (ADPCM from disc) + one-shot SFX in sound RAM. */
#ifndef SRDC_AUDIO_H
#define SRDC_AUDIO_H
#include <stdint.h>
void audio_init(void);
/* Play an AICA-ADPCM WAV (fmt 0x14) from disc as a looping stream.
 * loop_start/loop_end are in samples (0,0 = loop whole file). Returns 0 on success. */
int  audio_music_play(const char *path, uint32_t loop_start, uint32_t loop_end);
void audio_music_stop(void);
void audio_update(void);            /* call once per frame */
int  audio_sfx_load(const char *path);  /* returns handle or -1 */
void audio_sfx_play(int h, int vol, int pan);
#endif
