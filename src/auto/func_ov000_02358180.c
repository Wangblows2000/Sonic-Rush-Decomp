/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 AnimatorSprite_SetAnimation();
s32 Background_Draw();
extern u8 gDisplaySub[];

void func_ov000_02358180(u8 *p0) {
    if (*(u32 *)((u8 *)p0 + 0xb1c) != 1) {
        AnimatorSprite_SetAnimation(((u8 *)p0 + 0x5c4), 1);
        *(u32 *)((u8 *)p0 + 0xb1c) = 1;
        Background_Draw(((s32)((u8 *)p0 + 0x344) + ((*(u32 *)((u8 *)p0 + 0x14) + 0x3) << 6)), ((u8 *)p0 + 0x344));
        *(u16 *)((u8 *)gDisplaySub + 0x8) = 0;
        *(u16 *)((u8 *)gDisplaySub + 0xa) = 0x20;
    }
    *(u32 *)((u8 *)p0 + 0x5f0) = (*(u32 *)((u8 *)p0 + 0x5f0) & ~4);
}

#pragma thumb reset
