/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 AnimatorSprite_SetAnimation();

void func_ov000_02357cf8(s32 p0) {
    AnimatorSprite_SetAnimation(((u8 *)p0 + 0x60c), 3);
    *(u32 *)((u8 *)p0 + 0x638) = (*(u32 *)((u8 *)p0 + 0x638) | 4);
}

#pragma thumb reset
