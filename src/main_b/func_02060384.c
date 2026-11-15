#include <nitro/types.h>

#pragma thumb on

typedef struct { u32 unk0; u32 unk4; } UnkStruct_0207747c;
extern UnkStruct_0207747c data_0207747c[];
extern s16 data_022c4da0;
UnkStruct_0207747c *func_02060384(void) { return &data_0207747c[data_022c4da0]; }

#pragma thumb reset
