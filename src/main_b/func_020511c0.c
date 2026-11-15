#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x10]; u32 *unk10; } UnkTask;
extern UnkTask *data_022c4638;
void func_020511c0(void) { data_022c4638->unk10[6] = 1; }

#pragma thumb reset
