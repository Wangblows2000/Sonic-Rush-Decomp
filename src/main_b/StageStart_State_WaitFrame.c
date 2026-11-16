#include <nitro/types.h>

typedef struct { u32 unk0; u32 unk4; void (*fn)(void); } UnkStruct_0204bdf8;
extern UnkStruct_0204bdf8 *gTaskCurrent;
void StageStart_State_SetupResources(void);
void StageStart_State_WaitFrame(void) { gTaskCurrent->fn = StageStart_State_SetupResources; }
