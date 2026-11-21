#include <nitro/types.h>

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 e);
extern u32 data_0207f02c;
void TP_SetCallback(u32 v) { u32 e = OS_DisableInterrupts(); data_0207f02c = v; OS_RestoreInterrupts(e); }
