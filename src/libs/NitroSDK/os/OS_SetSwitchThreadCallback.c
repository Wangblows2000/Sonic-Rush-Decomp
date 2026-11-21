#include <nitro/types.h>

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 e);
typedef struct { u8 pad[0x10]; void *x10; } UnkStruct_0207d090;
extern UnkStruct_0207d090 data_0207d090;
void *OS_SetSwitchThreadCallback(void *v) { u32 e = OS_DisableInterrupts(); void *old = data_0207d090.x10; data_0207d090.x10 = v; OS_RestoreInterrupts(e); return old; }
