#include <nitro/types.h>

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 e);
void SNDi_CallAlarmHandler(u32 a);
void PxiFifoCallback(u32 a, u32 b) { u32 e = OS_DisableInterrupts(); SNDi_CallAlarmHandler(b); OS_RestoreInterrupts(e); }
