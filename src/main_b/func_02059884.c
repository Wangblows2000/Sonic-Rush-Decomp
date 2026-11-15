#include <nitro/types.h>

typedef struct { u8 unk0[0xe]; u8 unkE; u8 unkF[0x57]; u8 unk66; } UnkStruct_022c4680;
extern UnkStruct_022c4680 data_022c4680;
void func_02059884(s32 which, u32 v, BOOL clamp) { if (clamp && v <= 2) v = 2; switch (which) { case 0: data_022c4680.unkE = v; break; case 1: data_022c4680.unk66 = v; break; } }
