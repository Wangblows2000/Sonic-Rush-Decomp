#include <nitro/types.h>

typedef struct { u8 unk0[0x1c]; u8 unk1C; u8 unk1D; } UnkStruct_022c4560;
extern UnkStruct_022c4560 gGameState;
void StageStart_Create(s32 a);
void func_0204e4b8(void) { gGameState.unk1D++; StageStart_Create(3); }
