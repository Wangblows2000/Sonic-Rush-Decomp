#include <nitro/types.h>

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 e);
extern u16 OSi_UseVAlarm;
void OS_EndVAlarm(void) { u32 e = OS_DisableInterrupts(); if (OSi_UseVAlarm != 0) OSi_UseVAlarm = 0; OS_RestoreInterrupts(e); }
