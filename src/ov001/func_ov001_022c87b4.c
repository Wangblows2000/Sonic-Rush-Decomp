#include <nitro/types.h>

extern s32 data_ov001_0231f040;
void func_02042d34(s32 a);

#pragma thumb on
void func_ov001_022c87b4(void) {
    func_02042d34(0);
    data_ov001_0231f040 = 0;
}
#pragma thumb reset
