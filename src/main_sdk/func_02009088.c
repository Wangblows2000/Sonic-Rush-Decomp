#include <nitro/types.h>

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 e);
void func_0200976c(u32 a);
void func_02009088(u32 a, u32 b) { u32 e = OS_DisableInterrupts(); func_0200976c(b); OS_RestoreInterrupts(e); }
