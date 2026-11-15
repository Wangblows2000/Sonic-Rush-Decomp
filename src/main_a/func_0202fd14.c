#include <nitro/types.h>

BOOL func_0200e300(s32 *a, s32 *b);

s32 func_0202fd14(void) {
    s32 a, b;
    if (!func_0200e300(&a, &b)) {
        if (a == 1 && b == 1) return 2;
        if (a == 1) return 0;
        if (b == 1) return 1;
        return -1;
    }
    return 2;
}
