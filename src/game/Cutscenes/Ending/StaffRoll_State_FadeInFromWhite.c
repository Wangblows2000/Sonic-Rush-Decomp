/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 data_0208926c[];
s32 StaffRoll_SetMainBrightness();
extern u8 data_02087d44[];
s32 StaffRoll_SetSubBrightness();
void StaffRoll_State_ScrollCredits();
s32 StaffRoll_SetState();

void StaffRoll_State_FadeInFromWhite(u8 *p0) {
    s32 t0 = *(s16 *)((u8 *)data_0208926c + 8);
    if (t0 > 0) {
        s32 t1 = (*(u32 *)((u8 *)p0 + 0x14) & 1);
        if (t1 != 0) {
            StaffRoll_SetMainBrightness(((s32)((t0 - 0x1) << 0x10) >> 0x10), t1, t0);
        }
    }
    s32 t2 = *(s16 *)((u8 *)data_02087d44 + 8);
    if (t2 > 0) {
        s32 t3 = (*(u32 *)((u8 *)p0 + 0x14) & 1);
        if (t3 != 0) {
            StaffRoll_SetSubBrightness(((s32)((t2 - 0x1) << 0x10) >> 0x10), t3, t2);
        }
    }
    if (*(s16 *)((u8 *)data_0208926c + 8) <= 0) {
        if (*(s16 *)((u8 *)data_02087d44 + 8) <= 0) {
            StaffRoll_SetMainBrightness(0, data_02087d44);
            StaffRoll_SetSubBrightness(0);
            StaffRoll_SetState(p0, (u32)StaffRoll_State_ScrollCredits);
        }
    }
}

#pragma thumb reset
