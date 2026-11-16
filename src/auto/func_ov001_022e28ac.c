/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 ZoneMap_IsBlaze();
s32 func_ov001_022e2a44();
s32 func_ov001_022e299c();

s32 func_ov001_022e28ac(void) {
    s32 t0 = ZoneMap_IsBlaze();
    if (t0 == 0) {
        s32 t1 = func_ov001_022e2a44();
        if (t1 >= 0x22) {
            return 1;
        }
        if (t1 >= 0x21) {
            return 0xe;
        }
        if (t1 >= 0x1e) {
            return 0xd;
        }
        if (t1 >= 0x1b) {
            return 0xc;
        }
        if (t1 >= 0x16) {
            return 8;
        }
        if (t1 >= 0x11) {
            return 6;
        }
        if (t1 >= 0xb) {
            return 3;
        }
        if (t1 >= 5) {
            return 2;
        }
        return 1;
    }
    s32 t2 = func_ov001_022e299c(t0);
    if (t2 >= 0x21) {
        return 1;
    }
    if (t2 >= 0x20) {
        return 0xe;
    }
    if (t2 >= 0x1d) {
        return 0xd;
    }
    if (t2 >= 0x19) {
        return 0xc;
    }
    if (t2 >= 0x15) {
        return 8;
    }
    if (t2 >= 0x11) {
        return 6;
    }
    if (t2 >= 0xd) {
        return 3;
    }
    if (t2 >= 0xb) {
        return 2;
    }
    if (t2 >= 3) {
        return 1;
    }
    return 0;
}

#pragma thumb reset
