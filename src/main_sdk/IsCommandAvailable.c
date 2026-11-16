#include <nitro/types.h>

BOOL OS_IsRunOnEmulator(void);
u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 e);
BOOL IsCommandAvailable(void) { u32 e; u32 v; if (!OS_IsRunOnEmulator()) return 1; e = OS_DisableInterrupts(); *(vu32 *)0x4fff200 = 0x10; v = *(vu32 *)0x4fff200; OS_RestoreInterrupts(e); return v != 0; }
