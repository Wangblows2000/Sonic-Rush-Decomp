/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 data_022b4574[];
s32 func_02060564();
s32 func_020371c8();

void func_02060da0(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10);
    func_02060564(*(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10));
    s32 t1 = *(u32 *)t0;
    if (t1 != 0) {
        ((s32 (*)())t1)(t0, t1);
    }
    s32 t2 = (*(u32 *)((u8 *)t0 + 0x4) & 0x80000000);
    if (t2 != 0) {
        func_020371c8(*(u32 *)data_022b4574, t2);
    }
}

#pragma thumb reset
