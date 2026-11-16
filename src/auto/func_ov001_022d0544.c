/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_ov001_022d0220();
void func_ov001_022d0344();
s32 StaffRoll_SetPictureState();
void func_ov001_022d03f8();

s32 func_ov001_022d0544(s32 p0) {
    func_ov001_022d0220();
    s32 t0 = *(u32 *)((u8 *)p0 + 0xbdc);
    s32 v1 = t0;
    if (t0 > 1) {
        if (*(u32 *)((u8 *)p0 + 0xbe0) == 1) {
            s32 t2 = StaffRoll_SetPictureState(p0, (u32)func_ov001_022d0344);
            return t2;
        }
        s32 t3 = StaffRoll_SetPictureState(p0, (u32)func_ov001_022d03f8);
        v1 = t3;
    }
    return v1;
}

#pragma thumb reset
