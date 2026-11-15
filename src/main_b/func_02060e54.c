#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x10]; u32 unk10[1]; } UnkStruct_02060e54;
typedef struct { u8 unk0[0x10]; UnkStruct_02060e54 *unk10; } UnkTask;
extern UnkTask *data_022c4da4;
void func_02060e54(s32 i, u32 v) { if (data_022c4da4 != NULL) data_022c4da4->unk10->unk10[i] |= v; }

#pragma thumb reset
