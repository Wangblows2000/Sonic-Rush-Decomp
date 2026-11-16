/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 AnimatorSprite_SetAnimation();
void func_ov001_022d0c64();
s32 func_ov001_022d1404();

void func_ov001_022d0c84(s32 p0) {
    *(u32 *)((u8 *)p0 + 0xc10) = (*(u32 *)((u8 *)p0 + 0xc10) | 4);
    *(u32 *)((u8 *)p0 + 0xca0) = (*(u32 *)((u8 *)p0 + 0xca0) | 4);
    *(u32 *)((u8 *)p0 + 0xc10) = (*(u32 *)((u8 *)p0 + 0xc10) & ~0x80);
    *(u32 *)((u8 *)p0 + 0xca0) = (*(u32 *)((u8 *)p0 + 0xca0) & ~0x80);
    *(u32 *)((u8 *)p0 + 0xcbc) = 0xa0000;
    *(u32 *)((u8 *)p0 + 0xcc8) = 0x78000;
    *(u32 *)((u8 *)p0 + 0xcd4) = 0;
    *(u32 *)((u8 *)p0 + 0xcec) = 0;
    *(u32 *)((u8 *)p0 + 0xcc0) = 0xfffe2000;
    *(u32 *)((u8 *)p0 + 0xcc4) = (*(u32 *)((u8 *)p0 + 0xcbc) + 0x3c000);
    *(u32 *)((u8 *)p0 + 0xcd0) = 0x46000;
    AnimatorSprite_SetAnimation(((u8 *)p0 + 0xbe4), 0x1a, 0xfffe2000);
    func_ov001_022d1404(p0, (u32)func_ov001_022d0c64);
}

#pragma thumb reset
