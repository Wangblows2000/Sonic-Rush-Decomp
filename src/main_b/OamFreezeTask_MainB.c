#include <nitro/types.h>

extern void *gTaskCurrent;
void Oam_Freeze(s32 a);
void Task_Destroy(void *p);
void OamFreezeTask_MainB(void) { Oam_Freeze(1); Task_Destroy(gTaskCurrent); }
