#include <nitro/types.h>

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 e);
extern u16 data_0207d388;
void func_02007584(void) { u32 e = OS_DisableInterrupts(); if (data_0207d388 != 0) data_0207d388 = 0; OS_RestoreInterrupts(e); }
