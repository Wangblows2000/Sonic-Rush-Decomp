#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x34]; u16 unk34; u8 unk36[0xd2]; u16 unk108[1]; } UnkWork;
typedef struct { u8 unk0[0x10]; UnkWork *unk10; } UnkTask;
extern UnkTask *data_022c4638;
u16 func_02050ec0(s32 i) { UnkWork *w = data_022c4638->unk10; if (i >= w->unk34) return 0; return w->unk108[i]; }

#pragma thumb reset
