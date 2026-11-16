/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 AnimatorSprite_SetAnimation();
s32 AnimatorSprite_ProcessAnimation();

void PlayerSelectMenu_SetSprite654Anim3(s32 p0) {
    AnimatorSprite_SetAnimation(((u8 *)p0 + 0x654), 3);
    AnimatorSprite_ProcessAnimation(((u8 *)p0 + 0x654), 0, 0);
    *(u32 *)((u8 *)p0 + 0x680) = (*(u32 *)((u8 *)p0 + 0x680) & ~4);
    *(u32 *)((u8 *)p0 + 0x680) = (*(u32 *)((u8 *)p0 + 0x680) | 1);
    *(u32 *)((u8 *)p0 + 0x680) = (*(u32 *)((u8 *)p0 + 0x680) | 0x80);
}

#pragma thumb reset
