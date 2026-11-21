#include <nitro/types.h>

void MI_CpuCopy8(const void *src, void *dest, u32 size);

static inline u8 *func_02014498_Put(u8 *p, const void *src, u32 n)
{
    MI_CpuCopy8(src, p, n);
    return p + n;
}

s32 WBT_MakeReqUserDataPacket(u8 *buf, u16 bitmap, u32 c, const void *data, u32 e)
{
    u8 type = 0xa;
    u16 bm = bitmap;
    u8 *p = buf;
    p = func_02014498_Put(p, &type, 1);
    p = func_02014498_Put(p, &bm, 2);
    p = func_02014498_Put(p, &c, 1);
    p = func_02014498_Put(p, &e, 1);
    p = func_02014498_Put(p, data, 9);
    return p - buf;
}
