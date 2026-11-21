#include <nitro/types.h>

void *memcpy(void *dst, const void *src, u32 n) {
    u8 *d = dst;
    const u8 *s = src;
    if (n != 0) {
        do {
            *d++ = *s++;
        } while (--n != 0);
    }
    return dst;
}
