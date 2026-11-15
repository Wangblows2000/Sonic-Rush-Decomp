// cc: 1.2/sp2p3
#include <nitro/types.h>

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 e);
extern u8 *data_02080f1c;
void func_02016b6c(u32 v) { u32 e = OS_DisableInterrupts(); *(u32 *)(data_02080f1c + 0x14e4) = v; OS_RestoreInterrupts(e); }
