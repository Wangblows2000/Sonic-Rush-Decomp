#include <nitro/types.h>

typedef struct { u8 unk0[0x18]; u32 unk18; } UnkStruct_022c4560;
extern UnkStruct_022c4560 gGameState;
void Stage_ResetProgress(void);
void StageStart_Create(s32 a);
void StageEntry_StartVs(void) { Stage_ResetProgress(); gGameState.unk18 |= 0x20; StageStart_Create(3); }
