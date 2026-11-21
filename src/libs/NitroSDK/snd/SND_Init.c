#include <nitro/types.h>

extern u32 data_0207d3dc;
extern u8 data_0207d3e0[];
void OS_InitMutex(void *p); void SND_CommandInit(void); void SND_AlarmInit(void);
void SND_Init(void) { if (data_0207d3dc != 0) return; data_0207d3dc = 1; OS_InitMutex(data_0207d3e0); SND_CommandInit(); SND_AlarmInit(); }
