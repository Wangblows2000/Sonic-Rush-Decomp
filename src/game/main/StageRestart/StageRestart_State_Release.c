#include <nitro/types.h>

typedef struct { u32 unk0; u32 unk4; void (*fn)(void); } UnkTask;
extern UnkTask *gTaskCurrent;
void Stage_DestroyTasks(void);
void Stage_ReleaseZoneResources(void);
void Stage_ReleaseCommonResources(void);
void VRAMTransfer_Clear(void);
void ScreenTransfer_Clear(void);
void PaletteTransfer_Clear(void);
void StageRestart_State_SetupResources(void);
void StageRestart_State_Release(void) { Stage_DestroyTasks(); Stage_ReleaseZoneResources(); Stage_ReleaseCommonResources(); VRAMTransfer_Clear(); ScreenTransfer_Clear(); PaletteTransfer_Clear(); gTaskCurrent->fn = StageRestart_State_SetupResources; }
