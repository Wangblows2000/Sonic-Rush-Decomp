#include <nitro/types.h>

void OSi_UnLockCartridgeAlias(u16 id);
u32 OS_RestoreInterrupts(u32 e);
typedef struct { u32 locked; u32 irq; } UnkStruct_0201aa40;
void CTRDGi_UnlockByProcessor(u16 id, UnkStruct_0201aa40 *s) { if (s->locked == 0) OSi_UnLockCartridgeAlias(id); OS_RestoreInterrupts(s->irq); }
