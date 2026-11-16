/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 StaffRoll_AdvancePictureAnim();
s32 AnimatorSprite_SetAnimation();
void StaffRoll_PictureState_Blaze6();
s32 StaffRoll_SetPictureState();

void StaffRoll_PictureState_Blaze5(s32 p0) {
    StaffRoll_AdvancePictureAnim();
    *(u32 *)((u8 *)p0 + 0xc58) = (*(u32 *)((u8 *)p0 + 0xc58) & ~4);
    *(u32 *)((u8 *)p0 + 0xca0) = (*(u32 *)((u8 *)p0 + 0xca0) | 4);
    *(u32 *)((u8 *)p0 + 0xc10) = (*(u32 *)((u8 *)p0 + 0xc10) & ~0x80);
    *(u32 *)((u8 *)p0 + 0xc58) = (*(u32 *)((u8 *)p0 + 0xc58) & ~0x80);
    *(u32 *)((u8 *)p0 + 0xca0) = (*(u32 *)((u8 *)p0 + 0xca0) & ~0x80);
    *(u32 *)((u8 *)p0 + 0xcbc) = 0xa0000;
    *(u32 *)((u8 *)p0 + 0xcc8) = 0x78000;
    *(u32 *)((u8 *)p0 + 0xcd4) = 0;
    *(u32 *)((u8 *)p0 + 0xcec) = 0;
    AnimatorSprite_SetAnimation(((u8 *)p0 + 0xc2c), 0x20, 0x80, ((u8 *)p0 + 0xca0));
    StaffRoll_SetPictureState(p0, (u32)StaffRoll_PictureState_Blaze6);
}

#pragma thumb reset
