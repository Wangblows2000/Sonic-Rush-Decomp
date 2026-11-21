#include <nitro/types.h>

typedef struct { u8 unk0[0x1c]; u8 unk1C; u8 unk1D; } UnkStruct_022c4560;
extern UnkStruct_022c4560 gGameState;
typedef struct { u32 unk0; u32 unk4; void (*fn)(void); } UnkTask;
extern UnkTask *gTaskCurrent;
void Stage_InitDisplay(s32 a);
void Stage_InitDisplayBoss(s32 a);
void Stage_SetupCommonResources(void);
void Stage_SetupZoneResources(void);
void StageRestart_State_CreateStage(void);
void StageRestart_State_SetupResources(void) { if (gGameState.unk1C < 9 && gGameState.unk1D < 2) Stage_InitDisplay(1); else Stage_InitDisplayBoss(1); Stage_SetupCommonResources(); Stage_SetupZoneResources(); gTaskCurrent->fn = StageRestart_State_CreateStage; }
