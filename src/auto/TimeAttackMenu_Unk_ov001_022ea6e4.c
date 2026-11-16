/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 AnimatorSprite_SetAnimation();
s32 AnimatorSprite_ProcessAnimation();

void TimeAttackMenu_Unk_ov001_022ea6e4(s32 p0) {
    AnimatorSprite_SetAnimation(((u8 *)p0 + 0x858), 2);
    *(u32 *)((u8 *)p0 + 0xb38) = 2;
    AnimatorSprite_ProcessAnimation(((u8 *)p0 + 0x858), 0, 0);
    *(u32 *)((u8 *)p0 + 0x884) = (*(u32 *)((u8 *)p0 + 0x884) | 4);
    *(u32 *)((u8 *)p0 + 0x884) = (*(u32 *)((u8 *)p0 + 0x884) | 0x80);
    *(u32 *)((u8 *)p0 + 0x884) = (*(u32 *)((u8 *)p0 + 0x884) | 1);
}

#pragma thumb reset
