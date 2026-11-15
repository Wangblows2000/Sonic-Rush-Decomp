#include <nitro/types.h>

extern void *data_ov001_0231f054;
void func_ov001_022d76c4(void *p, s32 a, s32 b);

#pragma thumb on
void func_ov001_022d6dc0(void) {
    func_ov001_022d76c4(data_ov001_0231f054, 0, 0);
}
#pragma thumb reset
