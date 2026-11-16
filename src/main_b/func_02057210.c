#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x10]; u32 *unk10; } UnkTask;
extern UnkTask *gPlayerDemoTask;
void func_02057210(void) { if (gPlayerDemoTask != NULL) gPlayerDemoTask->unk10[9] = 0; }

#pragma thumb reset
