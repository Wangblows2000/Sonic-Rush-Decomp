#include <nitro/types.h>

void func_02007a04(u32 dmaNo, u32 dmaType);
void func_02007994(u32 dmaNo, u32 src, u32 size, u32 dir);
void func_01ff85cc(u32 dmaNo, u32 src, u32 dest, u32 ctrl);

void func_02008860(u32 dmaNo, u32 src, u32 dest, u32 size)
{
    func_02007a04(dmaNo, (u32)-1);
    func_02007994(dmaNo, src, size, 0x1000000);
    if (size == 0) {
        return;
    }
    {
        vu32 *dmaCntp = &((vu32 *)0x040000b0)[dmaNo * 3 + 2];
        while (*dmaCntp & 0x80000000) {
        }
    }
    func_01ff85cc(dmaNo, src, dest, 0xaf000001);
}
