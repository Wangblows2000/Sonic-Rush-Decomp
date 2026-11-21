/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 AnimatorSprite_ProcessAnimation();
s32 AnimatorSprite_Draw();

void ZoneMap_DrawBackButton(s32 p0) {
    s32 t0 = (*(u32 *)((u8 *)p0 + 0xa4) + 0x1b08);
    *(u16 *)((u8 *)t0 + 0x130) = 8;
    *(u16 *)((u8 *)t0 + 0x132) = 0x98;
    AnimatorSprite_ProcessAnimation((t0 + 0x12c), 0, 0);
    AnimatorSprite_Draw((t0 + 0x12c));
}

#pragma thumb reset
