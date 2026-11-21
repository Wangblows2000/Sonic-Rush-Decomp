/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 AnimatorSprite_ProcessAnimation();
s32 AnimatorSprite_Draw();

void PlayerSelectMenu_DrawSprite654(s32 p0, s32 p1, s32 p2) {
    *(u16 *)((u8 *)p0 + 0x658) = p1;
    *(u16 *)((u8 *)p0 + 0x65a) = p2;
    AnimatorSprite_ProcessAnimation(((u8 *)p0 + 0x654), 0, 0);
    AnimatorSprite_Draw(((u8 *)p0 + 0x654));
}

#pragma thumb reset
