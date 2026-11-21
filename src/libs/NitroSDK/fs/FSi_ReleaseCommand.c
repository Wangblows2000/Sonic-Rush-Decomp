#include <nitro/types.h>

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 e);
void FSi_CutFromList(void *p);
void OS_WakeupThread(void *q);
typedef struct { u8 pad[0xc]; u32 flags; u32 pad2; u32 result; u8 q[1]; } UnkStruct_0200aea4;
void FSi_ReleaseCommand(UnkStruct_0200aea4 *p, u32 r) { u32 e = OS_DisableInterrupts(); FSi_CutFromList(p); p->flags &= ~0x4f; p->result = r; OS_WakeupThread(p->q); OS_RestoreInterrupts(e); }
