#include <nitro/types.h>

extern void *gTaskCurrent;
void Task_Destroy(void *p);
void func_0204bf88(void);
void func_0204bbbc(void) { Task_Destroy(gTaskCurrent); func_0204bf88(); }
