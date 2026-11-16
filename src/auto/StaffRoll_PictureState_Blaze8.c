/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_ov001_022d0220();
void StaffRoll_PictureState_Blaze9();
s32 StaffRoll_SetPictureState();

void StaffRoll_PictureState_Blaze8(s32 p0) {
    func_ov001_022d0220();
    s32 t0 = *(u32 *)((u8 *)p0 + 0xcc0);
    if (t0 >= 0x50000) {
        *(u32 *)((u8 *)p0 + 0xcc0) = 0x50000;
        *(u32 *)((u8 *)p0 + 0xcd8) = 0;
        *(u32 *)((u8 *)p0 + 0xcf0) = 0;
        StaffRoll_SetPictureState(p0, (u32)StaffRoll_PictureState_Blaze9, t0);
    }
}

#pragma thumb reset
