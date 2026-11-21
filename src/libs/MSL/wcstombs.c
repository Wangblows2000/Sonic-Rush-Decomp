#include <nitro/types.h>

s32 wctomb(char *dst, u16 c);
char *strncpy(char *dst, const char *src, u32 n);

u32 wcstombs(char *dst, const u16 *src, u32 n) {
    u32 len = 0;
    s32 k;
    char buf[4];
    char *b;
    if (dst == NULL || src == NULL) return 0;
    b = buf;
    while (len <= n) {
        if (*src == 0) {
            dst[len] = 0;
            break;
        }
        k = wctomb(b, *src++);
        if (len + k > n) break;
        strncpy(dst + len, b, k);
        len += k;
    }
    return len;
}
