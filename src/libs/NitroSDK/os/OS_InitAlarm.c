#include <nitro/types.h>

extern u16 data_0207d37c;
typedef struct { u32 a; u32 b; } UnkStruct_0207d380;
extern UnkStruct_0207d380 data_0207d380;
void OSi_SetTimerReserved(u32 n);
u32 OS_DisableIrqMask(u32 m);
void OS_InitAlarm(void) {
    if (data_0207d37c != 0) return;
    data_0207d37c = 1;
    OSi_SetTimerReserved(1);
    data_0207d380.a = 0;
    data_0207d380.b = 0;
    OS_DisableIrqMask(0x10);
}
