#include <nitro/types.h>

void func_020533e0(s32 a, s32 b);
void func_020537b4(s32 a);

#pragma thumb on
void func_ov001_022e8d44(void) {
    func_020533e0(5, 0xb4);
    func_020537b4(7);
}
#pragma thumb reset
