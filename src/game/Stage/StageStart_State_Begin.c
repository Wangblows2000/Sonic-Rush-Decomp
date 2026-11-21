#include <nitro/types.h>

typedef struct { u32 unk0; u32 unk4; void (*fn)(void); } UnkTask;
extern UnkTask *gTaskCurrent;
void Touch_Enable(void);
void StageStart_State_InitCommonLoad(void);
void StageStart_State_Begin(void) { Touch_Enable(); gTaskCurrent->fn = StageStart_State_InitCommonLoad; }
