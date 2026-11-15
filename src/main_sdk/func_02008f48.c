#include <nitro/types.h>

BOOL func_02006338(void);
u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 e);
BOOL func_02008f48(void) { u32 e; u32 v; if (!func_02006338()) return 1; e = OS_DisableInterrupts(); *(vu32 *)0x4fff200 = 0x10; v = *(vu32 *)0x4fff200; OS_RestoreInterrupts(e); return v != 0; }
