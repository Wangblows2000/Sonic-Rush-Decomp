#include <nitro/types.h>

#pragma thumb on

typedef struct { u32 unk0; u32 unk4; } UnkStruct_0207747c;
extern UnkStruct_0207747c data_0207747c[];
extern s16 gTalkDemoId;
UnkStruct_0207747c *TalkDemo_GetEntry(void) { return &data_0207747c[gTalkDemoId]; }

#pragma thumb reset
