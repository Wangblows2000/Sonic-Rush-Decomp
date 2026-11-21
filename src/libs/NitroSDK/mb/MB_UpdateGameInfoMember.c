#include <nitro/types.h>

void MIi_CpuCopy16(const void *src, void *dst, u32 size);

void MB_UpdateGameInfoMember(u8 *obj, const void *src, u16 bitmap, u16 x)
{
    s32 i;
    u8 n = 1;
    MIi_CpuCopy16(src, obj + 0x35e, 0x14a);
    for (i = 0; i < 15; i++) {
        if (bitmap & (2 << i)) {
            n++;
        }
    }
    *(obj + 0x358) = n;
    *(u16 *)(obj + 0x35a) = bitmap | 1;
    *(u16 *)(obj + 0x35c) = x;
    (*(obj + 0x4b4))++;
}
