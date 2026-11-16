#ifndef OV000_D_BGCAM_H
#define OV000_D_BGCAM_H

#include <nitro/types.h>

typedef struct UnkStruct_BgCam {
    u8 pad0[0x2C];
    s32 x;
    s32 y;
    u8 pad34[0xC8 - 0x34];
    u32 flags;
} UnkStruct_BgCam;

extern UnkStruct_BgCam data_ov000_0236aad8;
extern UnkStruct_BgCam data_ov000_0236ab3c;

UnkStruct_BgCam *func_ov000_0233b62c(void);
UnkStruct_BgCam *func_ov000_0233b610(void);
void *Task_Create(void (*f)(void), void (*g)(void), s32 b, s32 c, s32 d, s32 e);

#endif
