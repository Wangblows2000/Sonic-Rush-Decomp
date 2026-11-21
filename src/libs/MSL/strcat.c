#include <nitro/types.h>

char *strcat(char *dst, const char *src) {
    const u8 *s = (const u8 *)src;
    u8 *d = (u8 *)dst;
    while (*d++) {
    }
    d--;
    while ((*d++ = *s++) != 0) {
    }
    return dst;
}
