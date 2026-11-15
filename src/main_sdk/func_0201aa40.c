// cc: 1.2/sp2p3
#include <nitro/types.h>

void OS_UnLockCartridge(u16 id);
u32 OS_RestoreInterrupts(u32 e);
typedef struct { u32 locked; u32 irq; } UnkStruct_0201aa40;
void func_0201aa40(u16 id, UnkStruct_0201aa40 *s) { if (s->locked == 0) OS_UnLockCartridge(id); OS_RestoreInterrupts(s->irq); }
