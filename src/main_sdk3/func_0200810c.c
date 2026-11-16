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

extern void func_02007994(u32 dmaNo, const void *src, u32 size, u32 a);
extern void func_02007b3c(u32 dmaNo);

extern void func_02005448(u32 dmaNo, void (*callback)(void), void *arg);
extern void func_02008098(void);
extern void func_01ff85cc(u32 dmaNo, u32 src, u32 dest, u32 ctrl);
extern u32 OS_ResetRequestIrqMask(u32 mask);

void func_0200810c(void)
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
        func_02005448(data_0207d3bc.dmaNo, func_02008098, NULL);
        func_01ff85cc(data_0207d3bc.dmaNo, src, 0x04000400, 0xc4400000 | (length >> 2));
        OS_ResetRequestIrqMask(0x200000);
    } else {
        func_01ff85cc(data_0207d3bc.dmaNo, src, 0x04000400, 0x84400000 | (length >> 2));
        OS_ResetRequestIrqMask(0x200000);
    }
}
