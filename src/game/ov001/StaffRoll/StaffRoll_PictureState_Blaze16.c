/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 StaffRoll_AdvancePictureAnim();
void StaffRoll_PictureState_BlazeExit1();
s32 StaffRoll_SetPictureState();
void StaffRoll_PictureState_BlazeRepeat();

s32 StaffRoll_PictureState_Blaze16(s32 p0) {
    StaffRoll_AdvancePictureAnim();
    s32 t0 = *(u32 *)((u8 *)p0 + 0xbdc);
    s32 v1 = t0;
    if (t0 > 1) {
        if (*(u32 *)((u8 *)p0 + 0xbe0) == 1) {
            s32 t2 = StaffRoll_SetPictureState(p0, (u32)StaffRoll_PictureState_BlazeExit1);
            return t2;
        }
        s32 t3 = StaffRoll_SetPictureState(p0, (u32)StaffRoll_PictureState_BlazeRepeat);
        v1 = t3;
    }
    return v1;
}

#pragma thumb reset
