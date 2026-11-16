#include <nitro/types.h>

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 e);
void OS_Terminate(void);
void MBi_CommEnd(void);
extern u8 *data_02080f1c;
void func_020186f8(void) { u32 e = OS_DisableInterrupts(); if (*(u32 *)(data_02080f1c + 0x1320) != 0) OS_Terminate(); MBi_CommEnd(); OS_RestoreInterrupts(e); }
