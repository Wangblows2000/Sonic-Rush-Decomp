#include <nitro/types.h>

typedef struct { u32 unk0; u32 unk4; void (*fn)(void); } UnkTask;
extern UnkTask *gTaskCurrent;
s32 StageLoader_LoadZoneFiles(void);
void StageStart_State_WaitFrame(void);
void StageStart_State_LoadZoneFiles(void) { if (StageLoader_LoadZoneFiles() == 1) gTaskCurrent->fn = StageStart_State_WaitFrame; }
