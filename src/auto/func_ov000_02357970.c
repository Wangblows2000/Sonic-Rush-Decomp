/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 AnimatorSprite_ProcessAnimation();
s32 AnimatorSprite_Draw();

void func_ov000_02357970(s32 p0) {
    AnimatorSprite_ProcessAnimation(((u8 *)p0 + 0x9fc), 0, 0);
    AnimatorSprite_Draw(((u8 *)p0 + 0x9fc));
}

#pragma thumb reset
