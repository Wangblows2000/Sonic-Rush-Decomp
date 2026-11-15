#include <nitro/types.h>

#pragma thumb on

typedef struct { u32 unk0; u8 unk4[0x14]; u32 unk18; } UnkWork;
typedef struct { u32 unk0; u32 unk4; void (*fn)(void); u32 unkC; UnkWork *unk10; } UnkTask;
extern UnkTask *data_022c45fc;
void func_0204efec(UnkWork *w);
void func_0204f9f8(void);
void func_0204fdc8(void) { UnkWork *w = data_022c45fc->unk10; w->unk0 = 0; w->unk18 = 5; func_0204efec(w); data_022c45fc->fn = func_0204f9f8; }

#pragma thumb reset
