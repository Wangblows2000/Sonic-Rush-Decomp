#include <nitro/types.h>

s32 __mbtowc_noconv(u16 *dst, const u8 *src, u32 n) {
    if (src == NULL) return 0;
    if (n == 0) return -1;
    if (dst != NULL) *dst = *src;
    if (*src == 0) return 0;
    return 1;
}
