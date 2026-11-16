#include <nitro/types.h>

extern s32 gVsMenuCancelled;
extern void *gTaskCurrent;
BOOL func_02050e2c(void);
void Task_Destroy(void *p);

#pragma thumb on
void func_ov001_022dabe4(void) {
    if (func_02050e2c()) {
        gVsMenuCancelled = 0;
        Task_Destroy(gTaskCurrent);
    }
}
#pragma thumb reset
