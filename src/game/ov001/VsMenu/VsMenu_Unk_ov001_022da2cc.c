/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 AnimatorSprite_SetAnimation();

void VsMenu_Unk_ov001_022da2cc(s32 p0, s32 p1) {
    AnimatorSprite_SetAnimation(((s32)((u8 *)p0 + 0x210) + (0x48 * p1)), ((u32)(p1 << 0x10) >> 0x10), ((u8 *)p0 + 0x210));
    *(u32 *)((u8 *)(p0 + (p1 << 2)) + 0xac8) = p1;
    *(u32 *)((u8 *)((s32)((u8 *)p0 + 0x210) + (0x48 * p1)) + 0x2c) = (*(u32 *)((u8 *)((s32)((u8 *)p0 + 0x210) + (0x48 * p1)) + 0x2c) & ~4);
}

#pragma thumb reset
