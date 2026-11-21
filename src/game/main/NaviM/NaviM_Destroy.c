#include <nitro/types.h>

extern void *gNaviMTask;
void Task_Destroy(void *p);
void NaviM_Destroy(void) { Task_Destroy(gNaviMTask); }
