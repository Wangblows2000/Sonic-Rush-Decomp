#include <nitro/types.h>

char *func_0202c4a4(char *dst, const char *src, u32 n) {
    u8 *d = (u8 *)dst;
    const u8 *s = (const u8 *)src;
    if (n != 0) {
        do {
            if ((*d++ = *s++) == 0) {
                while (--n != 0) {
                    *d++ = 0;
                }
                return dst;
            }
        } while (--n != 0);
    }
    return dst;
}
