#include <nitro/types.h>

extern s32 data_ov001_0231be10;
extern void *gTaskCurrent;
BOOL func_02050e2c(void);
void Task_Destroy(void *p);

#pragma thumb on
void func_ov001_022dabe4(void) {
    if (func_02050e2c()) {
        data_ov001_0231be10 = 0;
        Task_Destroy(gTaskCurrent);
    }
}
#pragma thumb reset
