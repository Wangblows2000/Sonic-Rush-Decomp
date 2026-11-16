/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_020371c8();
extern u8 data_ov001_023160f8[];
extern u8 data_0208922c[];
extern u8 data_0208922e[];
extern u8 data_02087d04[];
extern u8 data_02087d06[];

void func_ov001_022d2018(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x8);
    if (t0 != 0) {
        func_020371c8(t0);
        *(u32 *)((u8 *)p0 + 0x8) = 0;
    }
    s32 t1 = (*(u8 *)((u8 *)data_ov001_023160f8 + 0x4) << 2);
    *(u16 *)((u8 *)data_0208922c + t1) = 0;
    *(u16 *)((u8 *)data_0208922e + t1) = 0;
    s32 t2 = (*(u8 *)((u8 *)data_ov001_023160f8 + 0x7) << 2);
    *(u16 *)((u8 *)data_02087d04 + t2) = 0;
    *(u16 *)((u8 *)data_02087d06 + t2) = 0;
}

#pragma thumb reset
