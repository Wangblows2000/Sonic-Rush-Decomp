#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x10]; u32 unk10[1]; } UnkStruct_02060e70;
typedef struct { u8 unk0[0x10]; UnkStruct_02060e70 *unk10; } UnkTask;
extern UnkTask *data_022c4da4;
u32 func_02060e70(s32 i) { if (data_022c4da4 == NULL) return 0; return data_022c4da4->unk10->unk10[i]; }

#pragma thumb reset
