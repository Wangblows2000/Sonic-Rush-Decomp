#include <nitro/types.h>

typedef struct { u8 unk0[0x10]; u32 unk10; } UnkStruct_022c4560;
extern UnkStruct_022c4560 gGameState;
void Stage_ResetProgress(void);
void StageStart_Create(s32 a);
void StageEntry_StartTimeAttack(void) { Stage_ResetProgress(); gGameState.unk10 = 2; StageStart_Create(3); }
