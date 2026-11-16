#include <nitro/types.h>

extern void *data_ov001_0231f040;
extern s32 data_ov001_0231f044;
void Task_Destroy(void *p);

#pragma thumb on
void func_ov001_022c87f8(void) {
    if (data_ov001_0231f040 != NULL) {
        Task_Destroy(data_ov001_0231f040);
        data_ov001_0231f040 = NULL;
        data_ov001_0231f044 = 0;
    }
}
#pragma thumb reset
