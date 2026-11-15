#include <nitro/types.h>

typedef struct { u32 unk0; u32 unk4; u32 unk8; u32 unkC; } UnkWork;
typedef struct { u8 unk0[0x10]; UnkWork *unk10; } UnkTask;
extern UnkTask *data_022c4364;
u32 func_02042938(void) { if (data_022c4364 == NULL) return 0; return data_022c4364->unk10->unkC; }
