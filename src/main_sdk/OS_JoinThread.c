#include <nitro/types.h>

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 e);
void OS_SleepThread(void *q);
typedef struct { u8 pad[0x64]; u32 state; u8 pad2[0x28]; u8 q[1]; } UnkStruct_02005cc4;
void OS_JoinThread(UnkStruct_02005cc4 *p) { u32 e = OS_DisableInterrupts(); if (p->state != 2) OS_SleepThread(p->q); OS_RestoreInterrupts(e); }
