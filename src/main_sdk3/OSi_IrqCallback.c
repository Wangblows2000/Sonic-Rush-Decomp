#include <nitro/types.h>

typedef struct {
    void (*func)(void *);
    u32 enable;
    void *arg;
} UnkIrqCbInfo;

extern UnkIrqCbInfo data_0207d01c[];

extern u16 data_02077968[];
extern u8 data_027e0000[];

u32 OS_DisableIrqMask(u32 mask);

void OSi_IrqCallback(s32 dmaNo)
{
    u32 imask = (1UL << data_02077968[dmaNo]);
    void (*callback)(void *) = data_0207d01c[dmaNo].func;
    data_0207d01c[dmaNo].func = NULL;
    if (callback) {
        callback(data_0207d01c[dmaNo].arg);
    }
    *(vu32 *)((u32)data_027e0000 + 0x3ff8) |= imask;
    if (!data_0207d01c[dmaNo].enable) {
        (void)OS_DisableIrqMask(imask);
    }
}
