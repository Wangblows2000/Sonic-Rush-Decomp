#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x34]; u16 unk34; u8 unk36[0xb2]; u32 unkE8[1]; } UnkWork;
typedef struct { u8 unk0[0x10]; UnkWork *unk10; } UnkTask;
extern UnkTask *data_022c4638;
u32 func_02050ee4(s32 i) { UnkWork *w = data_022c4638->unk10; if (i >= w->unk34) return 0; return w->unkE8[i]; }

#pragma thumb reset
