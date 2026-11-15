#include <nitro/types.h>

void func_02025268(s32 a, void *p, s32 n);

#pragma thumb on
void func_ov001_022e2bbc(void) {
    u32 v = 0x20000000;
    func_02025268(0x2a, &v, 1);
}
#pragma thumb reset
