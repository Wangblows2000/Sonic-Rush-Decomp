#include <nitro/types.h>

#pragma thumb on

extern u32 data_0207c9f8[];
u32 TalkDemo_GetLanguageSuffix(s32 i) { return data_0207c9f8[i]; }

#pragma thumb reset
