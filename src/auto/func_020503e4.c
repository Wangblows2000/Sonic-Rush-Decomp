/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_02051660();
extern u8 data_022b4574[];
s32 func_020371c8();

void func_020503e4(void) {
    s32 t0 = func_02051660();
    if (t0 != 0) {
        s32 t1 = *(u32 *)*(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10);
        func_020371c8(*(u32 *)data_022b4574, *(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10));
        ((s32 (*)())t1)();
    }
}

#pragma thumb reset
