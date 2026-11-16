#include <nitro/types.h>

extern void *gTaskCurrent;
extern s32 data_ov001_0231f044;
u32 WH_GetBitmap(void);
void func_02042d34(s32 a);
void Task_Destroy(void *p);

#pragma thumb on
void func_ov001_022c8640(void) {
    if ((WH_GetBitmap() & ~1) == 0) {
        func_02042d34(0);
        Task_Destroy(gTaskCurrent);
        data_ov001_0231f044 = 0;
    }
}
#pragma thumb reset
