#include <nitro/types.h>

BOOL PM_GetBackLight(s32 *a, s32 *b);

s32 func_0202fd14(void) {
    s32 a, b;
    if (!PM_GetBackLight(&a, &b)) {
        if (a == 1 && b == 1) return 2;
        if (a == 1) return 0;
        if (b == 1) return 1;
        return -1;
    }
    return 2;
}
