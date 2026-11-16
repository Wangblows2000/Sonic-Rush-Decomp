/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 AnimatorSprite_SetAnimation();
void func_ov001_022d0ed0();
s32 func_ov001_022d1404();

void func_ov001_022d0ef0(s32 p0) {
    *(u32 *)((u8 *)p0 + 0xc10) = (*(u32 *)((u8 *)p0 + 0xc10) | 4);
    *(u32 *)((u8 *)p0 + 0xc10) = (*(u32 *)((u8 *)p0 + 0xc10) | 0x80);
    *(u32 *)((u8 *)p0 + 0xc58) = (*(u32 *)((u8 *)p0 + 0xc58) | 0x80);
    *(u32 *)((u8 *)p0 + 0xcd4) = 0;
    *(u32 *)((u8 *)p0 + 0xcec) = 0;
    *(u32 *)((u8 *)p0 + 0xcc0) = 0xfffd8000;
    *(u32 *)((u8 *)p0 + 0xccc) = 0x78000;
    *(u32 *)((u8 *)p0 + 0xcd8) = 0;
    *(u32 *)((u8 *)p0 + 0xcf0) = 0;
    AnimatorSprite_SetAnimation(((u8 *)p0 + 0xbe4), 0x16, 0x78000);
    func_ov001_022d1404(p0, (u32)func_ov001_022d0ed0);
}

#pragma thumb reset
