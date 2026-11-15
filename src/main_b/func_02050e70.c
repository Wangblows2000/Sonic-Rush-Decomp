#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x10]; u32 *unk10; } UnkTask;
extern UnkTask *data_022c4638;
void func_02050e70(void) { data_022c4638->unk10[5] = 1; }

#pragma thumb reset
