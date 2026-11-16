/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 data_022b4574[];
s32 func_02060e8c();
s32 func_020371c8();
s32 func_02060e70();

void func_02060220(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10);
    s32 t1 = func_02060e8c(*(u32 *)data_022b4574);
    if (t1 == 0) {
        func_020371c8(*(u32 *)data_022b4574);
        return;
    }
    s32 t2 = *(u32 *)t0;
    if (t2 != 0) {
        ((s32 (*)())t2)(t0, t2);
    }
    s32 t3 = func_02060e70(1);
    if ((t3 & 0x80000000) != 0) {
        func_020371c8(*(u32 *)data_022b4574, 0x80000000);
    }
}

#pragma thumb reset
