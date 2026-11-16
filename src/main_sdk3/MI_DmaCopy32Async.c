#include <nitro/types.h>

void MIi_CheckDma0SourceAddress(u32 dmaNo, u32 src, u32 size, u32 dir);
void MI_WaitDma(u32 dmaNo);
void OSi_EnterDmaCallback(u32 dmaNo, void (*callback)(void *), void *arg);
void MIi_DmaSetParams(u32 dmaNo, u32 src, u32 dest, u32 ctrl);

void MI_DmaCopy32Async(u32 dmaNo, const void *src, void *dest, u32 size, void (*callback)(void *), void *arg)
{
    MIi_CheckDma0SourceAddress(dmaNo, (u32)src, size, 0);
    if (size == 0) {
        if (callback) {
            callback(arg);
        }
    } else {
        MI_WaitDma(dmaNo);
        if (callback) {
            OSi_EnterDmaCallback(dmaNo, callback, arg);
            MIi_DmaSetParams(dmaNo, (u32)src, (u32)dest, 0xc4000000 | (size >> 2));
        } else {
            MIi_DmaSetParams(dmaNo, (u32)src, (u32)dest, 0x84000000 | (size >> 2));
        }
    }
}
