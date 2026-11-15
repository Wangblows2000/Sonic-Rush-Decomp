#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x10]; u32 *unk10; } UnkTask;
extern UnkTask *data_022c4da4;
u32 func_02060e20(void) { if (data_022c4da4 == NULL) return 0; return data_022c4da4->unk10[9]; }

#pragma thumb reset
