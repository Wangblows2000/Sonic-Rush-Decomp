/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 AnimatorSprite_ProcessAnimation();
s32 AnimatorSprite_Draw();

void PlayerSelectMenu_DrawSpriteA8C(u8 *p0, s32 p1, s32 p2) {
    if (*(u32 *)((u8 *)p0 + 0xc) != 2) {
        *(u16 *)((u8 *)p0 + 0xa90) = p1;
        *(u16 *)((u8 *)p0 + 0xa92) = p2;
        AnimatorSprite_ProcessAnimation(((u8 *)p0 + 0xa8c), 0, 0);
        AnimatorSprite_Draw(((u8 *)p0 + 0xa8c));
    }
}

#pragma thumb reset
