/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 AnimatorSprite_SetAnimation();
s32 AnimatorSprite_ProcessAnimation();

void PlayerSelectMenu_SetSprite804SelectAnim(s32 p0, s32 p1) {
    AnimatorSprite_SetAnimation(((u8 *)p0 + 0x804), ((u32)(((3 * p1) + 0x9) << 0x10) >> 0x10), 3);
    *(u32 *)((u8 *)p0 + 0x830) = (*(u32 *)((u8 *)p0 + 0x830) | 4);
    AnimatorSprite_ProcessAnimation(((u8 *)p0 + 0x804), 0, 0);
}

#pragma thumb reset
