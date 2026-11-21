#include <nitro/types.h>

void Mem_Copy(u32 src, u32 dst, u32 size);
void MI_DmaCopy32(u32 ch, u32 src, u32 dst, u32 size);
void MI_DmaCopy16(u32 ch, u32 src, u32 dst, u32 size);

void Mem_CopyRect(u32 src, u32 srcStride, u32 dst, u32 dstStride, u16 size, u16 rows)
{
    while (rows--) {
        u32 n = size;
        u32 s = (u32)(void *)src;
        u32 d0;
        if (src >= 0x2000000 && src < 0x2400000 && (d0 = dst) >= 0x2000000 && dst < 0x2400000) {
            Mem_Copy(src, dst, size);
        } else {
            u32 m;
            d0 = dst;
            if (((src | dst) & 3) == 0 && (m = n & ~3) != 0) {
                MI_DmaCopy32(3, s, dst, m);
                s = src + m;
                dst += m;
                n &= 3;
            }
            if (n != 0) {
                MI_DmaCopy16(3, s, dst, n);
            }
        }
        src += srcStride;
        dst = d0 + dstStride;
    }
}
