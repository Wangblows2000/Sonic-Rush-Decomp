#include <nitro/types.h>

s32 memcmp(const void *a, const void *b, u32 n) {
    const u8 *p1 = a;
    const u8 *p2 = b;
    if (n != 0) {
        do {
            if (*p1++ != *p2++) {
                return (p1[-1] < p2[-1]) ? -1 : 1;
            }
        } while (--n != 0);
    }
    return 0;
}
