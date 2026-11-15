#include <nitro/types.h>

#pragma thumb on

typedef struct { u32 unk0; u8 unk4[0x14]; u32 unk18; } UnkWork;
typedef struct { u32 unk0; u32 unk4; void (*fn)(void); u32 unkC; UnkWork *unk10; } UnkTask;
extern UnkTask *data_022c45fc;
void func_0204fc60(void);
void func_0204fe20(void) { UnkWork *w = data_022c45fc->unk10; w->unk0 = 0; w->unk18 = 1; data_022c45fc->fn = func_0204fc60; }

#pragma thumb reset
