/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 data_022c4680[];
s32 func_ov001_022e2ad8();
s32 func_ov001_022e2a44();
s32 func_ov001_022e299c();

s32 func_ov001_022e26d0(void) {
    s32 t0 = (*(u32 *)((u8 *)data_022c4680 + 0x8) & 0x1000);
    if (t0 != 0) {
        return 0;
    }
    s32 t1 = func_ov001_022e2ad8(0x1000, t0);
    if (t1 == 0) {
        s32 t2 = func_ov001_022e2a44();
        if (t2 >= 0x22) {
            return 8;
        }
        if (t2 >= 0x1f) {
            return 7;
        }
        if (t2 >= 0x1c) {
            return 6;
        }
        if (t2 >= 0x17) {
            return 5;
        }
        if (t2 >= 0x12) {
            return 4;
        }
        if (t2 >= 0xc) {
            return 3;
        }
        if (t2 >= 6) {
            return 2;
        }
        return 1;
    }
    s32 t3 = func_ov001_022e299c(t1);
    if (t3 >= 0x21) {
        return 8;
    }
    if (t3 >= 0x1e) {
        return 7;
    }
    if (t3 >= 0x1a) {
        return 6;
    }
    if (t3 >= 0x16) {
        return 5;
    }
    if (t3 >= 0x12) {
        return 4;
    }
    if (t3 >= 0xe) {
        return 3;
    }
    if (t3 >= 0xc) {
        return 2;
    }
    if (t3 >= 4) {
        return 1;
    }
    return 0;
}

#pragma thumb reset
