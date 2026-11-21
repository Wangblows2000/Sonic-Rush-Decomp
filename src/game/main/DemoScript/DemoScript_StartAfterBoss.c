#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x1c]; u8 unk1C; } UnkStruct_022c4560;
extern UnkStruct_022c4560 gGameState;
void DemoScript_Create(s32 a);
void DemoScript_StartAfterBoss(void) { if (gGameState.unk1C == 8) DemoScript_Create(2); else DemoScript_Create(1); }

#pragma thumb reset
