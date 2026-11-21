#include <nitro/types.h>

typedef struct { u8 unk0[0x10]; s32 unk10; u8 unk14[4]; u32 unk18; } UnkStruct_022c4560;
extern UnkStruct_022c4560 gGameState;
typedef struct { u32 unk0; u32 unk4; void (*fn)(void); } UnkTask;
extern UnkTask *gTaskCurrent;
void Stage_SetupCommonResources(void);
void Stage_SetupZoneResources(void);
void Stage_RestoreEventState(void);
void Stage_RestoreRestartCamera(void);
void Stage_AllocEventStateBuffers(void);
void StageStart_State_CreateStage(void);
void StageStart_State_SetupResources(void) { Stage_SetupCommonResources(); Stage_SetupZoneResources(); if (gGameState.unk10 == 0) { if (gGameState.unk18 & 0x40) { Stage_RestoreEventState(); Stage_RestoreRestartCamera(); } else Stage_AllocEventStateBuffers(); } gTaskCurrent->fn = StageStart_State_CreateStage; }
