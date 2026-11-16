/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 AnimatorSprite_SetAnimation();
s32 AnimatorSprite_ProcessAnimation();

void func_ov000_02358278(s32 p0) {
    if (*(u32 *)((u8 *)p0 + 0xb50) != 1) {
        AnimatorSprite_SetAnimation(((u8 *)p0 + 0x96c), 1);
        AnimatorSprite_ProcessAnimation(((u8 *)p0 + 0x96c), 0, 0);
        *(u32 *)((u8 *)p0 + 0xb50) = 1;
    }
}

#pragma thumb reset
