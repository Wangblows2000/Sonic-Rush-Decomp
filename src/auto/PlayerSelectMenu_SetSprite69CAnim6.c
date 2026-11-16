/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 AnimatorSprite_SetAnimation();

void PlayerSelectMenu_SetSprite69CAnim6(s32 p0) {
    AnimatorSprite_SetAnimation(((u8 *)p0 + 0x69c), 6);
    *(u32 *)((u8 *)p0 + 0x6c8) = (*(u32 *)((u8 *)p0 + 0x6c8) & ~4);
}

#pragma thumb reset
