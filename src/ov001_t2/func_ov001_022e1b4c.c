#include <nitro/types.h>

fx32 func_0200225c(fx32 a, fx32 b);
s32 func_02002978(fx32 t);

#pragma thumb on
s32 func_ov001_022e1b4c(s32 x0, s32 y0, s32 x1, s32 y1) {
    fx32 dx = (x1 - x0) << 12;
    fx32 dy = (y1 - y0) << 12;
    s32 a;

    if (dx == 0) {
        if (dy >= 0) {
            return 0;
        }
        return 0x8000;
    }
    if (dy == 0) {
        if (dx >= 0) {
            return 0x4000;
        }
        return 0xc000;
    }
    a = func_02002978(func_0200225c(dx < 0 ? -dx : dx, dy < 0 ? -dy : dy));
    if (dx >= 0) {
        if (dy < 0) {
            return 0x8000 - a;
        }
    } else {
        if (dy >= 0) {
            return 0x10000 - a;
        }
        a += 0x8000;
    }
    return a;
}
#pragma thumb reset
