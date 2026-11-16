/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

void func_ov001_022d0588();
s32 StaffRoll_SetPictureState();
s32 AnimatorSprite_SetAnimation();
void func_ov001_022d1a24();
s32 StaffRoll_SetState();

void func_ov001_022d1acc(s32 p0) {
    StaffRoll_SetPictureState(p0, (u32)func_ov001_022d0588);
    *(u32 *)((u8 *)p0 + 0xc10) = (*(u32 *)((u8 *)p0 + 0xc10) | 4);
    *(u32 *)((u8 *)p0 + 0xc58) = (*(u32 *)((u8 *)p0 + 0xc58) | 4);
    *(u32 *)((u8 *)p0 + 0xca0) = (*(u32 *)((u8 *)p0 + 0xca0) | 4);
    *(u32 *)((u8 *)p0 + 0xc10) = (*(u32 *)((u8 *)p0 + 0xc10) & ~0x80);
    *(u32 *)((u8 *)p0 + 0xc58) = (*(u32 *)((u8 *)p0 + 0xc58) & ~0x80);
    *(u32 *)((u8 *)p0 + 0xca0) = (*(u32 *)((u8 *)p0 + 0xca0) & ~0x80);
    *(u32 *)((u8 *)p0 + 0xcbc) = 0xbe000;
    *(u32 *)((u8 *)p0 + 0xcc8) = 0x78000;
    *(u32 *)((u8 *)p0 + 0xcc0) = 0x28000;
    *(u32 *)((u8 *)p0 + 0xccc) = 0x78000;
    *(u32 *)((u8 *)p0 + 0xcc4) = (*(u32 *)((u8 *)p0 + 0xcbc) + 0x14000);
    *(u32 *)((u8 *)p0 + 0xcd0) = 0x46000;
    AnimatorSprite_SetAnimation(((u8 *)p0 + 0xbe4), 0x1d, 0x28000, 0x78000);
    AnimatorSprite_SetAnimation(((u8 *)p0 + 0xc2c), 0x22);
    AnimatorSprite_SetAnimation(((u8 *)p0 + 0xc74), 0x23);
    *(u32 *)((u8 *)p0 + 0xcd4) = 0;
    *(u32 *)((u8 *)p0 + 0xcec) = 0;
    *(u32 *)((u8 *)p0 + 0xcd8) = 0;
    *(u32 *)((u8 *)p0 + 0xcf0) = 0;
    *(u32 *)((u8 *)p0 + 0xcdc) = 0;
    *(u32 *)((u8 *)p0 + 0xcf4) = 0;
    *(u32 *)((u8 *)p0 + 0xd04) = 1;
    StaffRoll_SetState(p0, (u32)func_ov001_022d1a24);
}

#pragma thumb reset
