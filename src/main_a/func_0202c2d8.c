#include <nitro/types.h>

char *func_0202c2d8(const char *str, s32 c) {
    const u8 *p = (const u8 *)str;
    u8 ch = c;
    u8 x;
    while ((x = *p++) != 0) {
        if (x == ch) return (char *)(p - 1);
    }
    if (ch != 0) return NULL;
    return (char *)(p - 1);
}
