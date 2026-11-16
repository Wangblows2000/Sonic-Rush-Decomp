/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 AnimatorSprite_SetAnimation();
void StaffRoll_State_SkipFadeIn();
s32 StaffRoll_SetState();

void StaffRoll_State_SkipSonic(s32 p0) {
    AnimatorSprite_SetAnimation(((u8 *)p0 + 0xbe4), 0x12);
    AnimatorSprite_SetAnimation(((u8 *)p0 + 0xc2c), 0x18);
    StaffRoll_SetState(p0, (u32)StaffRoll_State_SkipFadeIn);
}

#pragma thumb reset
