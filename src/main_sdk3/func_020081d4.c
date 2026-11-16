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
extern u32 OS_DisableInterrupts(void);
extern u32 OS_RestoreInterrupts(u32 state);
extern UnkIrqFunc OS_GetIrqFunction(u32 intrBit);
extern void func_02005520(u32 intrBit, UnkIrqFunc function);
extern u32 OS_EnableIrqMask(u32 mask);
extern void func_0200810c(void);

#define REG_GXSTAT (*(vu32 *)0x04000600)

void func_020081d4(u32 dmaNo, const void *src, u32 commandLength, UnkCallback_020081d4 callback, void *arg)
{
    u32 state;
    if (commandLength == 0) {
        if (callback) {
            callback(arg);
        }
        return;
    }
    while (data_0207d3bc.busy) {
    }
    while (!(((REG_GXSTAT & 0x7000000) >> 24) & 2)) {
    }
    data_0207d3bc.callback = callback;
    data_0207d3bc.busy = TRUE;
    data_0207d3bc.dmaNo = dmaNo;
    data_0207d3bc.src = src;
    data_0207d3bc.length = commandLength;
    data_0207d3bc.arg = arg;
    func_02007994(dmaNo, src, commandLength, 0);
    func_02007b3c(dmaNo);
    state = OS_DisableInterrupts();
    data_0207d3bc.fifoCond = (REG_GXSTAT & 0xc0000000) >> 30;
    data_0207d3bc.fifoFunc = OS_GetIrqFunction(0x200000);
    REG_GXSTAT = (REG_GXSTAT & ~0xc0000000) | 0x40000000;
    func_02005520(0x200000, func_0200810c);
    OS_EnableIrqMask(0x200000);
    func_0200810c();
    OS_RestoreInterrupts(state);
}
