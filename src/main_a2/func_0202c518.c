#include <nitro/types.h>

char *func_0202c518(char *dst, const char *src) {
    const u8 *s = (const u8 *)src;
    u8 *d = (u8 *)dst;
    u32 align = (u32)s & 3;
    u32 w;
    if (((u32)d & 3) != align) goto bytes;
    if (align != 0) {
        if ((*d = *s) == 0) return dst;
        for (align = 3 - align; align != 0; align--) {
            if ((*++d = *++s) == 0) return dst;
        }
        d++;
        s++;
    }
    w = *(const u32 *)s;
    if ((w + 0xFEFEFEFF) & 0x80808080) goto bytes;
    d -= 4;
    for (;;) {
        d += 4;
        *(u32 *)d = w;
        s += 4;
        w = *(const u32 *)s;
        if ((w + 0xFEFEFEFF) & 0x80808080) break;
    }
    d += 4;
bytes:
    if ((*d = *s) == 0) return dst;
    for (;;) {
        if ((*++d = *++s) == 0) return dst;
    }
}
