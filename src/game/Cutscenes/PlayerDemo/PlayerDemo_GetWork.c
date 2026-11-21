#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x10]; void *unk10; } UnkTask;
extern UnkTask *gPlayerDemoTask;
void *PlayerDemo_GetWork(void) { if (gPlayerDemoTask != NULL) return gPlayerDemoTask->unk10; return NULL; }

#pragma thumb reset
