/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 AnimatorSprite_SetAnimation();

void PlayerSelectMenu_SetSprite6E4Anim7(s32 p0) {
    AnimatorSprite_SetAnimation(((u8 *)p0 + 0x6e4), 7);
    *(u32 *)((u8 *)p0 + 0x710) = (*(u32 *)((u8 *)p0 + 0x710) & ~4);
}

#pragma thumb reset
