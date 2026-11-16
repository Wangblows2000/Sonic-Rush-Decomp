#include <nitro/types.h>

void NaviMsg_Destroy(void);
void func_02063f38(void);
void NaviM_Release(void);
void func_02054378(void);
void SysSound_Release(void);
void Touch_Disable(void);
void func_ov001_022c5c9c(void);

#pragma thumb on
void func_ov001_022c5fdc(void) {
    NaviMsg_Destroy();
    func_02063f38();
    NaviM_Release();
    func_02054378();
    SysSound_Release();
    Touch_Disable();
    func_ov001_022c5c9c();
}
#pragma thumb reset
