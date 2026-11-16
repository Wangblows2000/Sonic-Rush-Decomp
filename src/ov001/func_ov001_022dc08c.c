#include <nitro/types.h>

extern s32 gVsMenuCancelled;
extern void *gTaskCurrent;
BOOL func_0205109c(void);
void Task_Destroy(void *p);

#pragma thumb on
void func_ov001_022dc08c(void) {
    if (func_0205109c()) {
        gVsMenuCancelled = 0;
        Task_Destroy(gTaskCurrent);
    }
}
#pragma thumb reset
