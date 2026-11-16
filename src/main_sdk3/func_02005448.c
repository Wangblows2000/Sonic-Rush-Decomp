#include <nitro/types.h>

typedef struct {
    void (*func)(void *);
    u32 enable;
    void *arg;
} UnkIrqCbInfo;

extern UnkIrqCbInfo data_0207d01c[];

u32 OS_EnableIrqMask(u32 mask);

void func_02005448(u32 dmaNo, void (*callback)(void *), void *arg)
{
    u32 imask = (1UL << (8 + dmaNo));
    data_0207d01c[dmaNo].func = callback;
    data_0207d01c[dmaNo].arg = arg;
    data_0207d01c[dmaNo].enable = OS_EnableIrqMask(imask) & imask;
}
