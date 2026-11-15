#include <nitro/types.h>

void *func_0202a1e0(const void *src, s32 c, u32 n) {
    const u8 *p = src;
    u8 ch = c;
    if (n != 0) {
        do {
            if (*p++ == ch) return (void *)(p - 1);
        } while (--n != 0);
    }
    return NULL;
}
