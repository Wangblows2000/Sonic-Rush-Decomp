/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 AnimatorSprite_SetAnimation();
extern u8 gDisplaySub[];

void func_ov000_023581dc(s32 p0) {
    if (*(u32 *)((u8 *)p0 + 0xb1c) != 0) {
        AnimatorSprite_SetAnimation(((u8 *)p0 + 0x5c4), 0);
        *(u32 *)((u8 *)p0 + 0xb1c) = 0;
    }
    *(u32 *)((u8 *)p0 + 0x5f0) = (*(u32 *)((u8 *)p0 + 0x5f0) & ~4);
    *(u16 *)((u8 *)gDisplaySub + 0xa) = 0xfff8;
}

#pragma thumb reset
