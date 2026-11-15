#include <nitro/types.h>

s32 func_0202a12c(char *dst, u16 c);
char *func_0202c4a4(char *dst, const char *src, u32 n);

u32 func_0202a098(char *dst, const u16 *src, u32 n) {
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
        k = func_0202a12c(b, *src++);
        if (len + k > n) break;
        func_0202c4a4(dst + len, b, k);
        len += k;
    }
    return len;
}
