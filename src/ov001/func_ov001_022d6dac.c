#include <nitro/types.h>

extern void *data_ov001_0231f054;
void func_ov001_022d75dc(void *p, s32 a, s32 b);

#pragma thumb on
void func_ov001_022d6dac(void) {
    func_ov001_022d75dc(data_ov001_0231f054, 0, 0);
}
#pragma thumb reset
