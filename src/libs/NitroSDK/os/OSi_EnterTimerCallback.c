#include <nitro/types.h>

u32 OS_EnableIrqMask(u32 m);
typedef struct { void *fn; u32 enable; void *arg; } UnkStruct_020053fc;
extern UnkStruct_020053fc data_0207d04c[];
void OSi_EnterTimerCallback(u32 ch, void *fn, void *arg) {
    data_0207d04c[ch].fn = fn;
    data_0207d04c[ch].arg = arg;
    OS_EnableIrqMask(1 << (ch + 3));
    data_0207d04c[ch].enable = 1;
}
