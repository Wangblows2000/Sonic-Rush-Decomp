#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x10]; void *unk10; } UnkObj;
typedef struct { u8 unk0[0x24]; UnkObj *unk24; } UnkWork;
typedef struct { u8 unk0[0x10]; UnkWork *unk10; } UnkTask;
extern UnkTask *gPlayerDemoTask;
void *func_02057224(void) { UnkObj *o; if (gPlayerDemoTask == NULL) return NULL; o = gPlayerDemoTask->unk10->unk24; if (o == NULL) return NULL; return o->unk10; }

#pragma thumb reset
