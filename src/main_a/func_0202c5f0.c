#include <nitro/types.h>

s32 func_0202c5f0(const u8 *s) {
    s32 n = -1;
    do {
        n++;
    } while (*s++ != 0);
    return n;
}
