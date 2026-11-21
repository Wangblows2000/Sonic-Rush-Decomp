/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 StaffRoll_AdvancePictureAnim();
s32 AnimatorSprite_SetAnimation();
void StaffRoll_PictureState_Blaze13();
s32 StaffRoll_SetPictureState();

void StaffRoll_PictureState_Blaze12(s32 p0) {
    StaffRoll_AdvancePictureAnim();
    *(u32 *)((u8 *)p0 + 0xc10) = (*(u32 *)((u8 *)p0 + 0xc10) | 4);
    *(u32 *)((u8 *)p0 + 0xc58) = (*(u32 *)((u8 *)p0 + 0xc58) | 4);
    *(u32 *)((u8 *)p0 + 0xca0) = (*(u32 *)((u8 *)p0 + 0xca0) | 4);
    *(u32 *)((u8 *)p0 + 0xc10) = (*(u32 *)((u8 *)p0 + 0xc10) & ~0x80);
    *(u32 *)((u8 *)p0 + 0xc58) = (*(u32 *)((u8 *)p0 + 0xc58) & ~0x80);
    *(u32 *)((u8 *)p0 + 0xca0) = (*(u32 *)((u8 *)p0 + 0xca0) & ~0x80);
    *(u32 *)((u8 *)p0 + 0xcc0) = 0x50000;
    *(u32 *)((u8 *)p0 + 0xccc) = 0x78000;
    *(u32 *)((u8 *)p0 + 0xcd8) = 0;
    *(u32 *)((u8 *)p0 + 0xcf0) = 0;
    AnimatorSprite_SetAnimation(((u8 *)p0 + 0xc2c), 0x22, ((u8 *)p0 + 0xc58), ((u8 *)p0 + 0xca0));
    StaffRoll_SetPictureState(p0, (u32)StaffRoll_PictureState_Blaze13);
}

#pragma thumb reset
