/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 data_022c45fc[];
s32 func_020371c8();

void func_0205002c(void) {
    s32 t0 = *(u32 *)data_022c45fc;
    if (t0 != 0) {
        func_020371c8(t0);
        *(u32 *)data_022c45fc = 0;
    }
}

#pragma thumb reset
