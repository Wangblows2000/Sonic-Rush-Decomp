#include <nitro/types.h>

typedef struct { u8 unk0[0x1c]; u8 unk1C; u8 unk1D; u16 unk1E; } UnkStruct_022c4560;
extern UnkStruct_022c4560 gGameState;
void DemoPlay_Create(u16 a);
void DemoPlay_Start(void) { DemoPlay_Create(gGameState.unk1E); }
