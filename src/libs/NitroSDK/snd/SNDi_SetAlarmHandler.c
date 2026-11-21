#include <nitro/types.h>

typedef struct { void *cb; void *arg; u8 id; u8 pad[3]; } UnkStruct_020097cc;
extern UnkStruct_020097cc data_0207eec0[];
u8 SNDi_SetAlarmHandler(u32 i, void *cb, void *arg) { UnkStruct_020097cc *p = &data_0207eec0[i]; p->cb = cb; p->arg = arg; p->id++; return p->id; }
