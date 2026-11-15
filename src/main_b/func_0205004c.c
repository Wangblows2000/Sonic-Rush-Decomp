#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x10]; u32 *unk10; } UnkTask;
extern UnkTask *data_022c45fc;
void func_0205004c(void) { if (data_022c45fc != NULL) data_022c45fc->unk10[0x12] = 1; }

#pragma thumb reset
