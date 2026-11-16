/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_ov001_022d0070();
s32 func_ov001_022d0158();
extern u8 data_0208926c[];
extern u8 data_02087d44[];
void func_ov001_022d1ec8();
s32 StaffRoll_SetState();

void func_ov001_022d1a24(u8 *p0) {
    func_ov001_022d0070();
    if (*(u32 *)((u8 *)p0 + 0x14) == 1) {
        s32 t0 = func_ov001_022d0158(2);
        *(u32 *)((u8 *)p0 + 0xbd0) = t0;
        *(u32 *)((u8 *)*(u32 *)((u8 *)*(u32 *)((u8 *)p0 + 0xbd0) + 0x10) + 0x8) = 0x10;
        *(volatile u32 *)0x4000000 = ((*(volatile u32 *)0x4000000 & -7937) | 0x1800);
        *(volatile u32 *)0x4001000 = ((*(volatile u32 *)0x4001000 & -7937) | 0x800);
    }
    s32 t1 = *(s16 *)((u8 *)data_0208926c + 8);
    if (t1 < 0) {
        *(u16 *)((u8 *)data_0208926c + 0x8) = (t1 + 0x1);
    }
    s32 t2 = *(s16 *)((u8 *)data_02087d44 + 8);
    if (t2 < 0) {
        *(u16 *)((u8 *)data_02087d44 + 0x8) = (t2 + 0x1);
    }
    if (*(s16 *)((u8 *)data_0208926c + 8) >= 0) {
        if (*(s16 *)((u8 *)data_02087d44 + 8) >= 0) {
            *(u16 *)((u8 *)data_0208926c + 0x8) = 0;
            *(u16 *)((u8 *)data_02087d44 + 0x8) = 0;
            StaffRoll_SetState(p0, (u32)func_ov001_022d1ec8, data_0208926c);
            *(u32 *)((u8 *)p0 + 0x14) = 0xf0;
        }
    }
}

#pragma thumb reset
