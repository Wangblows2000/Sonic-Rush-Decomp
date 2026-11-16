#include <nitro/types.h>

extern u8 *gStaffRollWork;
void Task_Destroy(void *p);

#pragma thumb on
void func_ov001_022d0128(s32 i) {
    if (((void **)(gStaffRollWork + 0xbc8))[i] != NULL) {
        Task_Destroy(((void **)(gStaffRollWork + 0xbc8))[i]);
        ((void **)(gStaffRollWork + 0xbc8))[i] = NULL;
    }
}
#pragma thumb reset
