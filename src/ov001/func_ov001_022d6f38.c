#include <nitro/types.h>

void SysSound_Release(void);
void func_ov001_022d4fa0(void);

#pragma thumb on
void func_ov001_022d6f38(void) {
    SysSound_Release();
    func_ov001_022d4fa0();
}
#pragma thumb reset
