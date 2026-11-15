#include <nitro/types.h>

typedef struct { u8 unk0[0x10]; u32 *unk10; } UnkTask;
extern UnkTask *data_022c4364;
void func_02042324(void) { data_022c4364->unk10[9] = 0; }
