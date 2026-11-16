#include <nitro/types.h>

extern u8 *data_ov001_0231f5a8;
extern void *gTaskCurrent;
void Task_Destroy(void *p);
void func_ov001_02305aec(void);

void func_ov001_02305cd8(void) {
    *(s32 *)(data_ov001_0231f5a8 + 0x5d8) -= 1;
    if (*(s32 *)(data_ov001_0231f5a8 + 0x5d8) > 0) {
        return;
    }
    Task_Destroy(gTaskCurrent);
    *(s32 *)(data_ov001_0231f5a8 + 0x10) = 0;
    func_ov001_02305aec();
}
