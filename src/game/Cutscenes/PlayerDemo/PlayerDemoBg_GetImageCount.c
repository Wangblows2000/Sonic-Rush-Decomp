#include <nitro/types.h>

#pragma thumb on

typedef struct { u32 unk0; u32 unk4; } UnkStruct_020763e8;
extern UnkStruct_020763e8 data_020763e8[];
extern s16 gPlayerDemoId;
u32 PlayerDemoBg_GetImageCount(void) { s32 i = gPlayerDemoId; if (i < 0 || i >= 44) return 0; return data_020763e8[i].unk0; }

#pragma thumb reset
