/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 data_02076c20[];
extern u8 data_0207cb44[];
s32 func_0203879c();

s32 func_020603dc(s32 p0) {
    if ((s32)p0 == 0) {
        return 0;
    }
    s32 t0 = *(u32 *)((u8 *)data_02076c20 + ((s32)p0 << 2));
    if (t0 < 0) {
        return 0;
    }
    s32 t1 = func_0203879c(data_0207cb44, ((u32)(t0 << 0x10) >> 0x10), -1);
    return t1;
}

#pragma thumb reset
