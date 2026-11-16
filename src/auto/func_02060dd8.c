/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_02060480();
s32 func_02060e70();
s32 func_020609e0();
s32 func_020608d8();
s32 func_020605a4();
s32 func_02060824();
s32 func_02038914();
extern u8 data_022c4da4[];

void func_02060dd8(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x10);
    func_02060480(*(u32 *)((u8 *)*(u32 *)((u8 *)p0 + 0x10) + 0x24));
    s32 t1 = func_02060e70(0);
    if ((t1 & 1) == 0) {
        func_020609e0(t0, 1);
    } else {
        func_020608d8(t0);
    }
    func_020605a4(t0);
    func_02060824(t0);
    func_02038914();
    *(u32 *)data_022c4da4 = 0;
}

#pragma thumb reset
