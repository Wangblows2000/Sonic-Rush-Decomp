/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_ov001_022d0070();
s32 _fflt();
s32 _fdiv();
s32 func_0202e5e4();
s32 _fmul();
s32 _ffix();
s32 StaffRoll_SetMainBrightness();
void StaffRoll_State_ExtraShowFinalPicture();
s32 StaffRoll_SetState();

void StaffRoll_State_ExtraFadeOut(u8 *p0) {
    *(u32 *)((u8 *)p0 + 0xbb0) = 0;
    *(u32 *)((u8 *)p0 + 0xbb4) = 0;
    func_ov001_022d0070(p0, 0xbb4, 0);
    s32 t0 = *(u32 *)((u8 *)p0 + 0x14);
    if (t0 <= 0x90) {
        s32 t1 = _fflt(t0);
        s32 t2 = _fdiv(t1, 0x43100000);
        s32 t3 = func_0202e5e4(t2, 0x3f800000);
        s32 v4 = t2;
        if (t3 != 0) {
            v4 = 0x3f800000;
        }
        _fmul(0xc1800000, v4);
        _ffix();
        StaffRoll_SetMainBrightness();
        return;
    }
    StaffRoll_SetState(p0, (u32)StaffRoll_State_ExtraShowFinalPicture);
}

#pragma thumb reset
