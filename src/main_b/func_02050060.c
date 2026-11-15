#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x10]; u32 *unk10; } UnkTask;
extern UnkTask *data_022c45fc;
void func_02050060(u32 v) { if (data_022c45fc != NULL) data_022c45fc->unk10[5] |= v; }

#pragma thumb reset
