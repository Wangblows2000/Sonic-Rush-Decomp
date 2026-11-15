/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 data_022c4560[];
s32 func_02051628();
s32 func_02055140();

void func_02052458(s32 p0, s32 p1) {
    *(u32 *)((u8 *)data_022c4560 + 0x10) = 1;
    *(u8 *)((u8 *)data_022c4560 + 0x1c) = p0;
    *(u8 *)((u8 *)data_022c4560 + 0x1d) = p1;
    s32 t0 = func_02051628(1, p1, data_022c4560, 1);
    s32 t1 = func_02055140();
}

#pragma thumb reset
