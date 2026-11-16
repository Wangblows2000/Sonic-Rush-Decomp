/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 AnimatorSprite_ProcessAnimation();
s32 AnimatorSprite_Draw();

void func_ov000_02357994(s32 p0, s32 p1, s32 p2) {
    *(u16 *)((u8 *)p0 + 0x9b8) = p1;
    *(u16 *)((u8 *)p0 + 0x9ba) = p2;
    AnimatorSprite_ProcessAnimation(((u8 *)p0 + 0x9b4), 0, 0);
    AnimatorSprite_Draw(((u8 *)p0 + 0x9b4));
}

#pragma thumb reset
