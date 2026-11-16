#include <nitro/types.h>

typedef void (*UnkCallback_020081d4)(void *arg);
typedef void (*UnkIrqFunc)(void);

typedef struct {
    vu32 busy;
    u32 dmaNo;
    const void *src;
    u32 length;
    UnkCallback_020081d4 callback;
    void *arg;
    u32 fifoCond;
    UnkIrqFunc fifoFunc;
} UnkGXDmaInfo_020081d4;

extern UnkGXDmaInfo_020081d4 data_0207d3bc;

extern void MIi_CheckDma0SourceAddress(u32 dmaNo, const void *src, u32 size, u32 a);
extern void MI_WaitDma(u32 dmaNo);

extern void OSi_EnterDmaCallback(u32 dmaNo, void (*callback)(void), void *arg);
extern void MIi_DMACallback(void);
extern void MIi_DmaSetParams(u32 dmaNo, u32 src, u32 dest, u32 ctrl);
extern u32 OS_ResetRequestIrqMask(u32 mask);

void MIi_FIFOCallback(void)
{
    u32 src;
    u32 length;
    length = data_0207d3bc.length;
    if (length == 0) {
        return;
    }
    if (length >= 0x1d8) {
        length = 0x1d8;
    }
    src = (u32)data_0207d3bc.src;
    data_0207d3bc.length -= length;
    data_0207d3bc.src = (const void *)(src + length);
    if (data_0207d3bc.length == 0) {
        OSi_EnterDmaCallback(data_0207d3bc.dmaNo, MIi_DMACallback, NULL);
        MIi_DmaSetParams(data_0207d3bc.dmaNo, src, 0x04000400, 0xc4400000 | (length >> 2));
        OS_ResetRequestIrqMask(0x200000);
    } else {
        MIi_DmaSetParams(data_0207d3bc.dmaNo, src, 0x04000400, 0x84400000 | (length >> 2));
        OS_ResetRequestIrqMask(0x200000);
    }
}
