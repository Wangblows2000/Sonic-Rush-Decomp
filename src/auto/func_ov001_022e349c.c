/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 AnimatorSprite_ProcessAnimation();
s32 AnimatorSprite_Draw();

void func_ov001_022e349c(s32 p0) {
    s32 t0 = (*(u32 *)((u8 *)p0 + 0xa4) + 0x1b08);
    *(u16 *)((u8 *)t0 + 0x178) = 8;
    *(u16 *)((u8 *)t0 + 0x17a) = 0x98;
    AnimatorSprite_ProcessAnimation((t0 + 0x174), 0, 0);
    AnimatorSprite_Draw((t0 + 0x174));
}

#pragma thumb reset
