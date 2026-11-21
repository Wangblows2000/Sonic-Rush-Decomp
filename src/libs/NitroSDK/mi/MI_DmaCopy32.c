#include <nitro/types.h>

void MIi_CheckDma0SourceAddress(u32 dmaNo, u32 src, u32 size, u32 dir);
void MIi_DmaSetParams_wait(u32 dmaNo, u32 src, u32 dest, u32 ctrl);

void MI_DmaCopy32(u32 dmaNo, const void *src, void *dest, u32 size)
{
    vu32 *dmaCntp;

    MIi_CheckDma0SourceAddress(dmaNo, (u32)src, size, 0);
    if (size == 0) {
        return;
    }
    dmaCntp = &((vu32 *)0x040000b0)[dmaNo * 3 + 2];
    while (*dmaCntp & 0x80000000) {
    }
    MIi_DmaSetParams_wait(dmaNo, (u32)src, (u32)dest, 0x84000000 | (size >> 2));
    while (*dmaCntp & 0x80000000) {
    }
}
