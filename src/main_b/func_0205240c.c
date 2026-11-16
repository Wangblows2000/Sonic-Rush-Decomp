#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x1c]; u8 unk1C; u8 unk1D; u16 unk1E; } UnkStruct_022c4560;
extern UnkStruct_022c4560 gGameState;
void DemoScript_StartScript3(void);
void func_0205240c(void) { gGameState.unk1C = 8; gGameState.unk1D = 2; DemoScript_StartScript3(); }

#pragma thumb reset
