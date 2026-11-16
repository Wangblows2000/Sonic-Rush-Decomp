#include <nitro/types.h>

typedef struct { u32 flags; } UnkWork;
typedef struct { u32 unk0; u32 unk4; void (*fn)(void); u32 unkC; UnkWork *unk10; } UnkTask;
extern UnkTask *gTaskCurrent;
void StageLoader_Init(s32 a);
void StageStart_State_LoadZoneFiles(void);
void StageStart_State_WaitFrame(void);
void StageStart_State_InitZoneLoad(void) { UnkWork *w = gTaskCurrent->unk10; StageLoader_Init(0); if (w->flags & 2) gTaskCurrent->fn = StageStart_State_LoadZoneFiles; else gTaskCurrent->fn = StageStart_State_WaitFrame; }
