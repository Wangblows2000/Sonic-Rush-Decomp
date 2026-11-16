/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 AnimatorSprite_ProcessAnimation();
s32 AnimatorSprite_Draw();

void func_ov001_022e9fa8(s32 p0, s32 p1) {
    *(u16 *)((u8 *)((s32)((u8 *)p0 + 0x228) + (0x48 * (s32)p1)) + 0x4) = *(u32 *)((u8 *)((s32)p0 + (0x28 * (s32)p1)) + 0x8);
    *(u16 *)((u8 *)((s32)((u8 *)p0 + 0x228) + (0x48 * (s32)p1)) + 0x6) = 0;
    AnimatorSprite_ProcessAnimation(((s32)((u8 *)p0 + 0x228) + (0x48 * (s32)p1)), 0, 0);
    if (*(s16 *)((u8 *)((s32)((u8 *)p0 + 0x228) + (0x48 * (s32)p1)) + 4) < 0xc0) {
        AnimatorSprite_Draw(((s32)((u8 *)p0 + 0x228) + (0x48 * (s32)p1)));
    }
}

#pragma thumb reset
