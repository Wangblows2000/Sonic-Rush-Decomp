#include <nitro/types.h>

void func_02050c88(s32 a);

#pragma thumb on
void func_ov001_022c8d70(u8 *p) {
    func_02050c88(*(s32 *)(p + 0xc));
}
#pragma thumb reset
