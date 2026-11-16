#ifndef MAIN_A2_MIC_H
#define MAIN_A2_MIC_H

#include <nitro/types.h>

typedef struct UnkMic {
    u32 buf;
    u16 size;
    u16 unk6;
    u32 prev;
    u32 cur;
    u16 count;
    u16 unk12;
    u32 flags;
} UnkMic;

extern UnkMic data_0224ff00;

#endif
