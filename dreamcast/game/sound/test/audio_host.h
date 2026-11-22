/* host stand-in for engine/audio.c (see audio_host.c) */
#ifndef AUDIO_HOST_H
#define AUDIO_HOST_H
#include <stdint.h>
#include "../../engine/audio.h"
typedef struct {
    char log[16384];
    uint32_t gen[3];
    char music[128];
    int music_on, music_stop_frames;
    int mvol[2], mframes[2];
    struct { char path[128]; int on, paused, vol, stop_frames; } j[2];
    int preloads;
} AudioHost;
extern AudioHost g_ah;
void audio_host_reset_log(void);
#endif
