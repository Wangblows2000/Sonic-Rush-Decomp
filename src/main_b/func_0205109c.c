#include <nitro/types.h>

#pragma thumb on

typedef struct { u32 unk0; u32 unk4; u32 unk8; s32 unkC; } UnkWork;
typedef struct { u8 unk0[0x10]; UnkWork *unk10; } UnkTask;
extern UnkTask *data_022c4638;
BOOL func_0205109c(void) { s32 v; if (data_022c4638 == NULL) v = 7; else v = data_022c4638->unk10->unkC; if (v < 7) return FALSE; return TRUE; }

#pragma thumb reset
