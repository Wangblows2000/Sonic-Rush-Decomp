#include <nitro/types.h>

void MI_CpuCopy8(const void *src, void *dest, u32 size);

static inline u8 *func_0201442c_Put(u8 *p, const void *src, u32 n)
{
    MI_CpuCopy8(src, p, n);
    return p + n;
}

s32 func_0201442c(u8 *buf, u16 bitmap, u32 c)
{
    u8 type = 0xb;
    u16 bm = bitmap;
    u8 *p = buf;
    p = func_0201442c_Put(p, &type, 1);
    p = func_0201442c_Put(p, &bm, 2);
    p = func_0201442c_Put(p, &c, 1);
    return p - buf;
}
