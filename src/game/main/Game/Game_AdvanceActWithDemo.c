#include <nitro/types.h>

typedef struct { u8 unk0[0x1c]; u8 unk1C; u8 unk1D; } UnkStruct_022c4560;
extern UnkStruct_022c4560 gGameState;
void DemoScript_StartScript0(void);
void Game_AdvanceActWithDemo(void) { gGameState.unk1D++; DemoScript_StartScript0(); }
