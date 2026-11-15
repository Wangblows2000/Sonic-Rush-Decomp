#include <nitro/types.h>

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 e);
void func_0200bf1c(void *p);
void func_02005bd4(void *q);
typedef struct { u8 pad[0xc]; u32 flags; u32 pad2; u32 result; u8 q[1]; } UnkStruct_0200aea4;
void func_0200aea4(UnkStruct_0200aea4 *p, u32 r) { u32 e = OS_DisableInterrupts(); func_0200bf1c(p); p->flags &= ~0x4f; p->result = r; func_02005bd4(p->q); OS_RestoreInterrupts(e); }
