#include <nitro/types.h>

s32 func_ov000_022da7c8(s32 a, s32 b, BOOL dir) {
    if (dir) {
        if (a >= b)
            return a - b;
        return 0x5fe43 - (b - a);
    }
    if (a <= b)
        return b - a;
    return 0x5fe43 - (a - b);
}
