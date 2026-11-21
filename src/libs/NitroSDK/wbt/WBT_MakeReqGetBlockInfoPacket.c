#include <nitro/types.h>

void MI_CpuCopy8(const void *src, void *dest, u32 size);
void MI_CpuFill8(void *dest, u8 data, u32 size);

static inline u8 *func_020146f8_Put(u8 *p, const void *src, u32 n)
{
    MI_CpuCopy8(src, p, n);
    return p + n;
}

s32 WBT_MakeReqGetBlockInfoPacket(u8 *buf, u16 bitmap, u32 c, u32 d, u32 e)
{
    u8 type = 6;
    u16 bm = bitmap;
    u8 *p = buf;
    p = func_020146f8_Put(p, &type, 1);
    p = func_020146f8_Put(p, &bm, 2);
    p = func_020146f8_Put(p, &c, 1);
    p = func_020146f8_Put(p, &d, 4);
    p = func_020146f8_Put(p, &e, 4);
    return p - buf;
}
