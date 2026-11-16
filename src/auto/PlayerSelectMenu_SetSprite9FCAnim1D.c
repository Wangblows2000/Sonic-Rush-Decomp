/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 AnimatorSprite_SetAnimation();

void PlayerSelectMenu_SetSprite9FCAnim1D(s32 p0) {
    AnimatorSprite_SetAnimation(((u8 *)p0 + 0x9fc), 0x1d);
    *(u32 *)((u8 *)p0 + 0xa28) = (*(u32 *)((u8 *)p0 + 0xa28) | 4);
}

#pragma thumb reset
