#include <nitro/types.h>

typedef struct {
    vu32 isBusy;  /* 0x00 */
    u32 dmaNo;    /* 0x04 */
    u8 pad8[8];
    void (*callback)(void *); /* 0x10 */
    void *arg;    /* 0x14 */
} UnkGXDma_02007fd8;

extern UnkGXDma_02007fd8 data_0207d3bc;

void MIi_CheckAnotherAutoDMA(u32 dmaNo, u32 dmaType);
void MIi_CheckDma0SourceAddress(u32 dmaNo, u32 src, u32 size, u32 dir);
void MI_WaitDma(u32 dmaNo);
void OSi_EnterDmaCallback(u32 dmaNo, void (*callback)(void *), void *arg);
void MIi_DmaSetParams(u32 dmaNo, u32 src, u32 dest, u32 ctrl);
void MIi_DMAFastCallback(void *arg);

void MI_SendGXCommandAsyncFast(u32 dmaNo, const void *src, u32 commandLength, void (*callback)(void *), void *arg)
{
    if (commandLength == 0) {
        if (callback) {
            callback(arg);
        }
        return;
    }
    while (data_0207d3bc.isBusy) {
    }
    data_0207d3bc.isBusy = TRUE;
    data_0207d3bc.dmaNo = dmaNo;
    data_0207d3bc.callback = callback;
    data_0207d3bc.arg = arg;
    MIi_CheckAnotherAutoDMA(dmaNo, 0x38000000);
    MIi_CheckDma0SourceAddress(dmaNo, (u32)src, commandLength, 0);
    MI_WaitDma(dmaNo);
    OSi_EnterDmaCallback(dmaNo, MIi_DMAFastCallback, NULL);
    MIi_DmaSetParams(dmaNo, (u32)src, 0x04000400, 0xfc400000 | (commandLength >> 2));
}
