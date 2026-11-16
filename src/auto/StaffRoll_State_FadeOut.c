/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_ov001_022d0070();
extern u8 data_0208926c[];
s32 StaffRoll_SetMainBrightness();
extern u8 data_02087d44[];
s32 StaffRoll_SetSubBrightness();
void StaffRoll_State_End();
s32 StaffRoll_SetState();
void StaffRoll_State_WaitEnd();

void StaffRoll_State_FadeOut(u8 *p0) {
    s32 t0 = *(u32 *)p0;
    if (t0 != 2) {
        func_ov001_022d0070(p0, t0);
    }
    s32 t1 = *(s16 *)((u8 *)data_0208926c + 8);
    if (t1 > -16) {
        s32 t2 = (*(u32 *)((u8 *)p0 + 0x14) & 1);
        if (t2 != 0) {
            StaffRoll_SetMainBrightness(((s32)((t1 - 0x1) << 0x10) >> 0x10), t2, t1);
        }
    }
    s32 t3 = *(s16 *)((u8 *)data_02087d44 + 8);
    if (t3 > -16) {
        s32 t4 = (*(u32 *)((u8 *)p0 + 0x14) & 1);
        if (t4 != 0) {
            StaffRoll_SetSubBrightness(((s32)((t3 - 0x1) << 0x10) >> 0x10), t4, t3);
        }
    }
    if (*(s16 *)((u8 *)data_0208926c + 8) <= -16) {
        s32 t5 = *(s16 *)((u8 *)data_02087d44 + 8);
        if (t5 <= -16) {
            StaffRoll_SetMainBrightness(-16, t5, data_02087d44);
            StaffRoll_SetSubBrightness(-16);
            if (*(u32 *)p0 == 2) {
                StaffRoll_SetState(p0, (u32)StaffRoll_State_End);
                return;
            }
            StaffRoll_SetState(p0, (u32)StaffRoll_State_WaitEnd);
        }
    }
}

#pragma thumb reset
