#include <nitro/types.h>

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 state);
void MI_WaitDma(u32 dmaNo);
void OSi_EnterDmaCallback(u32 dmaNo, void (*callback)(void *), void *arg);
void MIi_DmaSetParams_noInt(u32 dmaNo, u32 src, u32 dest, u32 ctrl);

void MI_DmaFill32Async(u32 dmaNo, void *dest, u32 data, u32 size, void (*callback)(void *), void *arg)
{
    if (size == 0) {
        if (callback) {
            callback(arg);
        }
    } else {
        MI_WaitDma(dmaNo);
        if (callback) {
            OSi_EnterDmaCallback(dmaNo, callback, arg);
            {
                u32 enabled = OS_DisableInterrupts();
                vu32 *src = (vu32 *)0x040000e0 + dmaNo;
                *src = data;
                MIi_DmaSetParams_noInt(dmaNo, (u32)src, (u32)dest, 0xc5000000 | (size >> 2));
                (void)OS_RestoreInterrupts(enabled);
            }
        } else {
            {
                u32 enabled = OS_DisableInterrupts();
                vu32 *src = (vu32 *)0x040000e0 + dmaNo;
                *src = data;
                MIi_DmaSetParams_noInt(dmaNo, (u32)src, (u32)dest, 0x85000000 | (size >> 2));
                (void)OS_RestoreInterrupts(enabled);
            }
        }
    }
}
