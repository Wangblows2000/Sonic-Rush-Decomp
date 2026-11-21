/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 AnimatorSprite_SetAnimation();
s32 AnimatorSprite_ProcessAnimation();

void TimeAttackMenu_Unk_ov001_022ea734(s32 p0) {
    AnimatorSprite_SetAnimation(((u8 *)p0 + 0x810), 3);
    *(u32 *)((u8 *)p0 + 0xb34) = 3;
    AnimatorSprite_ProcessAnimation(((u8 *)p0 + 0x810), 0, 0);
    *(u32 *)((u8 *)p0 + 0x83c) = (*(u32 *)((u8 *)p0 + 0x83c) & ~4);
    *(u32 *)((u8 *)p0 + 0x83c) = (*(u32 *)((u8 *)p0 + 0x83c) & ~1);
}

#pragma thumb reset
