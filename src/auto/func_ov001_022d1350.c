/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 AnimatorSprite_SetAnimation();
void func_ov001_022d1330();
s32 StaffRoll_SetPictureState();

void func_ov001_022d1350(s32 p0) {
    *(u32 *)((u8 *)p0 + 0xc10) = (*(u32 *)((u8 *)p0 + 0xc10) | 4);
    *(u32 *)((u8 *)p0 + 0xc58) = (*(u32 *)((u8 *)p0 + 0xc58) | 4);
    s32 t0 = (*(u32 *)((u8 *)p0 + 0xc10) & ~0x80);
    *(u32 *)((u8 *)p0 + 0xc10) = t0;
    *(u32 *)((u8 *)p0 + 0xc58) = (*(u32 *)((u8 *)p0 + 0xc58) & ~0x80);
    *(u32 *)((u8 *)p0 + 0xcbc) = 0x46000;
    *(u32 *)((u8 *)p0 + 0xcc8) = 0x78000;
    *(u32 *)((u8 *)p0 + 0xcc0) = 0xb4000;
    *(u32 *)((u8 *)p0 + 0xccc) = 0x78000;
    *(u32 *)((u8 *)p0 + 0xcd4) = 0;
    *(u32 *)((u8 *)p0 + 0xcd8) = 0;
    AnimatorSprite_SetAnimation(((u8 *)p0 + 0xbe4), 0x12, 0x78000, t0);
    AnimatorSprite_SetAnimation(((u8 *)p0 + 0xc2c), 0x18);
    StaffRoll_SetPictureState(p0, (u32)func_ov001_022d1330);
}

#pragma thumb reset
