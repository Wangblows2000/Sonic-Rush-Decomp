#include <nitro/types.h>

extern s32 data_ov001_0231be10;
extern void *gTaskCurrent;
BOOL func_0205109c(void);
void Task_Destroy(void *p);

#pragma thumb on
void func_ov001_022dc08c(void) {
    if (func_0205109c()) {
        data_ov001_0231be10 = 0;
        Task_Destroy(gTaskCurrent);
    }
}
#pragma thumb reset
