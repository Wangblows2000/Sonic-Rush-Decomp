/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_ov001_022e18ec();
s32 func_ov001_022e18a4();
s32 func_ov001_022c5fdc();
extern u8 data_022b4574[];
s32 func_020371c8();
s32 func_ov001_022c5c94();
s32 func_ov001_022c5c80();

void func_ov001_022c5fa0(void) {
    s32 t0 = func_ov001_022e18ec();
    if (t0 != 0) {
        s32 t1 = func_ov001_022e18a4();
        func_ov001_022c5fdc();
        func_020371c8(*(u32 *)data_022b4574);
        if (t1 < 0) {
            func_ov001_022c5c94();
            return;
        }
        func_ov001_022c5c80(t1);
    }
}

#pragma thumb reset
