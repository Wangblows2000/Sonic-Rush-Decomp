#include <nitro/types.h>

typedef struct UnkWork { void (*fn)(struct UnkWork *); u8 unk4[8]; u32 unkC; u8 unk10[7]; u8 unk17; u8 unk18[2]; u16 unk1A; } UnkWork;
typedef struct { u8 unk0[0x10]; UnkWork *unk10; } UnkTask;
extern UnkTask *data_022b4574;
void func_0204138c(UnkWork *w);
void func_02041298(void) { UnkWork *w = data_022b4574->unk10; if (w->unk1A == 240) { w->unk1A++; w->unkC = 3; w->fn = func_0204138c; } else if (w->unk1A < 240) { w->unk1A++; } w->fn(w); w->unk17 = 0; }
