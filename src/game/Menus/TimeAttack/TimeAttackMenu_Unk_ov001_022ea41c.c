/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 AnimatorSprite_SetAnimation();

void TimeAttackMenu_Unk_ov001_022ea41c(s32 p0) {
    if (*(u32 *)((u8 *)p0 + 0xb50) != 0) {
        AnimatorSprite_SetAnimation(((u8 *)p0 + 0xa08), 0);
        *(u32 *)((u8 *)p0 + 0xb50) = 0;
    }
    *(u32 *)((u8 *)p0 + 0xa34) = (*(u32 *)((u8 *)p0 + 0xa34) & ~4);
}

#pragma thumb reset
