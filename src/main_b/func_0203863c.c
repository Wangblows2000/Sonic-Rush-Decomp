#include <nitro/types.h>

extern void *gTaskCurrent;
void Oam_Freeze(s32 a);
void Task_Destroy(void *p);
void func_0203863c(void) { Oam_Freeze(1); Task_Destroy(gTaskCurrent); }
