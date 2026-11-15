#include <nitro/types.h>

s32 func_0202c314(const char *s1, const char *s2, u32 n) {
    const u8 *p1 = (const u8 *)s1;
    const u8 *p2 = (const u8 *)s2;
    u8 c1, c2;
    if (n != 0) {
        do {
            c2 = *p2++;
            c1 = *p1++;
            if (c1 != c2) return c1 - c2;
            if (c1 == 0) break;
        } while (--n != 0);
    }
    return 0;
}
