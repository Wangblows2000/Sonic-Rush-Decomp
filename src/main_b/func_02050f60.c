#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x10]; u16 *unk10; } UnkTask;
extern UnkTask *data_022c4638;
u16 func_02050f60(void) { return data_022c4638->unk10[0x1a]; }

#pragma thumb reset
