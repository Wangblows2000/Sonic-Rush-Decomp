#include <nitro/types.h>

void *func_0202a224(void *dst, const void *src, u32 n) {
    u8 *d = dst;
    const u8 *s = src;
    if (n != 0) {
        do {
            *d++ = *s++;
        } while (--n != 0);
    }
    return dst;
}
