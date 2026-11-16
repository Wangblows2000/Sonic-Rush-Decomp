#include <nitro/types.h>

void NaviMsg_Destroy(void);
void func_02063f38(void);
void NaviM_Release(void);
void func_02054378(void);
void Task_DestroyRange(s32 a, s32 b);

#pragma thumb on
void func_ov001_022e3398(void) {
    NaviMsg_Destroy();
    func_02063f38();
    NaviM_Release();
    func_02054378();
    Task_DestroyRange(0x3000, 0x3000);
}
#pragma thumb reset
