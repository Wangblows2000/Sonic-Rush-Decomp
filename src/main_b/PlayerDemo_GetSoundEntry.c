#include <nitro/types.h>

#pragma thumb on

typedef struct { u32 unk0; u32 unk4; } UnkStruct_02076544;
extern UnkStruct_02076544 data_02076544[];
extern s16 gPlayerDemoId;
UnkStruct_02076544 *PlayerDemo_GetSoundEntry(void) { return &data_02076544[gPlayerDemoId]; }

#pragma thumb reset
