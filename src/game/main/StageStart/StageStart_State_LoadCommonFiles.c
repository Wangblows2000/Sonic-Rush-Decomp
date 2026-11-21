#include <nitro/types.h>

typedef struct { u32 unk0; u32 unk4; void (*fn)(void); } UnkTask;
extern UnkTask *gTaskCurrent;
s32 StageLoader_LoadCommonFiles(void);
void StageStart_State_InitZoneLoad(void);
void StageStart_State_LoadCommonFiles(void) { if (StageLoader_LoadCommonFiles() == 1) gTaskCurrent->fn = StageStart_State_InitZoneLoad; }
