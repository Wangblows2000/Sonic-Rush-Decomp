#include <nitro/types.h>

extern void *gTaskCurrent;
void Task_Destroy(void *p);
void Stage_Create(void);
void StageStart_State_CreateStage(void) { Task_Destroy(gTaskCurrent); Stage_Create(); }
