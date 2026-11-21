#include <nitro/types.h>

typedef struct { s32 unk0; } UnkStruct_022c4560;
extern UnkStruct_022c4560 gGameState;
extern u8 data_02074bc4[];
extern u8 data_02074bd0[];
u8 StoryProgress_GetZoneAt(u32 i) { u8 *t; if (i > 8) i = 8; if (gGameState.unk0 != 1) t = data_02074bc4; else t = data_02074bd0; return t[(u8)i]; }
