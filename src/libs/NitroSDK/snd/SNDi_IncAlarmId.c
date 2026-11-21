#include <nitro/types.h>

typedef struct { u8 pad[8]; u8 id; u8 pad2[3]; } UnkStruct_020097fc;
extern UnkStruct_020097fc data_0207eec0[];
void SNDi_IncAlarmId(u32 i) { UnkStruct_020097fc *p = &data_0207eec0[i]; p->id++; }
