/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 AnimatorSprite_SetAnimation();

void PlayerSelectMenu_SetSpriteA8CAnim0(s32 p0) {
    if (*(u32 *)((u8 *)p0 + 0xb60) != 0) {
        AnimatorSprite_SetAnimation(((u8 *)p0 + 0xa8c), 0);
        *(u32 *)((u8 *)p0 + 0xb60) = 0;
    }
    *(u32 *)((u8 *)p0 + 0xab8) = (*(u32 *)((u8 *)p0 + 0xab8) & ~4);
}

#pragma thumb reset
