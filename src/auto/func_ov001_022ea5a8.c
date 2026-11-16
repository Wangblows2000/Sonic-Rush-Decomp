/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 AnimatorSprite_SetAnimation();
extern u8 data_022c4560[];
extern u8 data_ov001_023166c4[];
extern u8 data_ov001_023166bc[];

void func_ov001_022ea5a8(s32 p0, s32 p1) {
    if ((s32)p1 >= 7) {
        if (*(u32 *)((u8 *)p0 + 0xb54) != 8) {
            AnimatorSprite_SetAnimation(((u8 *)p0 + 0xa50), 8);
            *(u32 *)((u8 *)p0 + 0xb54) = 8;
        }
        *(u32 *)((u8 *)p0 + 0xa7c) = (*(u32 *)((u8 *)p0 + 0xa7c) & ~4);
        return;
    }
    s32 v0 = *(u32 *)data_022c4560;
    if (*(u32 *)data_022c4560 == 0) {
        v0 = (s32)data_ov001_023166c4;
    } else {
        v0 = (s32)data_ov001_023166bc;
    }
    s32 t1 = ((u32)((*(u8 *)((u8 *)v0 + (s32)p1) + 0x5) << 0x10) >> 0x10);
    if (*(u32 *)((u8 *)p0 + 0xb3c) != t1) {
        AnimatorSprite_SetAnimation(((u8 *)p0 + 0x8a0), t1);
        *(u32 *)((u8 *)p0 + 0xb3c) = t1;
    }
    *(u32 *)((u8 *)p0 + 0x8cc) = (*(u32 *)((u8 *)p0 + 0x8cc) & ~4);
}

#pragma thumb reset
