#include <nitro/types.h>

#pragma thumb on

typedef struct { u32 unk0; u32 unk4; } UnkStruct_02057038;
extern UnkStruct_02057038 data_020768b8[];
u32 PolyDemo_GetModelFlags(s32 i) { return data_020768b8[i].unk0; }

#pragma thumb reset
