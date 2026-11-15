#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x10]; u32 *unk10; } UnkTask;
extern UnkTask *data_022c467c;
void func_02057210(void) { if (data_022c467c != NULL) data_022c467c->unk10[9] = 0; }

#pragma thumb reset
