/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 AnimatorSprite_SetAnimation();
void func_ov001_022d1a24();
s32 StaffRoll_SetState();

void func_ov001_022d1bec(s32 p0) {
    AnimatorSprite_SetAnimation(((u8 *)p0 + 0xbe4), 0x12);
    AnimatorSprite_SetAnimation(((u8 *)p0 + 0xc2c), 0x18);
    StaffRoll_SetState(p0, (u32)func_ov001_022d1a24);
}

#pragma thumb reset
