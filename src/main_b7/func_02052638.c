#include <nitro/types.h>

void func_02052638(const u16 *src, s32 srcStride, u16 *dst, s32 dstStride, u16 w, u16 h, s16 add) {
    const u16 *s;
    u16 *d;
    u16 n;
    u16 y;
    y = h;
    while (y-- != 0) {
        n = w;
        s = src;
        d = dst;
        for (; n != 0; n--, s++, d++) {
            *d = *s + add;
        }
        src = (const u16 *)((const u8 *)src + srcStride);
        dst = (u16 *)((u8 *)dst + dstStride);
    }
}
