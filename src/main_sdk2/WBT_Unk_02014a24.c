#include <nitro/types.h>

void MI_CpuCopy8(const void *src, void *dest, u32 size);

static inline u8 *func_02014a24_Put(u8 *p, const void *src, u32 n)
{
    MI_CpuCopy8(src, p, n);
    return p + n;
}

s32 WBT_Unk_02014a24(u8 *buf, u16 bitmap, u32 c)
{
    u8 type = 1;
    u16 bm = bitmap;
    u8 *p = buf;
    p = func_02014a24_Put(p, &type, 1);
    p = func_02014a24_Put(p, &bm, 2);
    p = func_02014a24_Put(p, &c, 1);
    return p - buf;
}
