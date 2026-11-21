/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 AnimatorSprite_SetAnimation();

void PlayerSelectMenu_SetSprite9B4ConfirmAnim(u8 *p0) {
    s32 t0 = ((u32)(((*(u32 *)((u8 *)p0 + 0x14) << 1) + 0x3) << 0x10) >> 0x10);
    if (*(u32 *)((u8 *)p0 + 0xb54) != t0) {
        AnimatorSprite_SetAnimation(((u8 *)p0 + 0x9b4), t0);
        *(u32 *)((u8 *)p0 + 0x9e0) = (*(u32 *)((u8 *)p0 + 0x9e0) | 4);
        *(u32 *)((u8 *)p0 + 0xb54) = t0;
    }
}

#pragma thumb reset
