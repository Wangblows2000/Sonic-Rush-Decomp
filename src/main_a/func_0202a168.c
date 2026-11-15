#include <nitro/types.h>

s32 func_0202a168(u16 *dst, const u8 *src, u32 n) {
    if (src == NULL) return 0;
    if (n == 0) return -1;
    if (dst != NULL) *dst = *src;
    if (*src == 0) return 0;
    return 1;
}
