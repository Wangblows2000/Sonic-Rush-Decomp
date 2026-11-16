#include <nitro/types.h>

void func_02007994(u32 dmaNo, u32 src, u32 size, u32 dir);
void func_01ff8558(u32 dmaNo, u32 src, u32 dest, u32 ctrl);

void func_02007da4(u32 dmaNo, const void *src, void *dest, u32 size)
{
    vu32 *dmaCntp;

    func_02007994(dmaNo, (u32)src, size, 0);
    if (size == 0) {
        return;
    }
    dmaCntp = &((vu32 *)0x040000b0)[dmaNo * 3 + 2];
    while (*dmaCntp & 0x80000000) {
    }
    func_01ff8558(dmaNo, (u32)src, (u32)dest, 0x84000000 | (size >> 2));
    while (*dmaCntp & 0x80000000) {
    }
}
