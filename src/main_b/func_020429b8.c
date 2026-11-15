#include <nitro/types.h>

typedef struct { u8 unk0[0x10]; u16 *unk10; } UnkTask;
extern UnkTask *data_022c4364;
u16 func_020429b8(void) { return data_022c4364->unk10[10]; }
