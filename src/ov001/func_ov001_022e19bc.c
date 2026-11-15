#include <nitro/types.h>

void func_0205247c(s32 a, s32 b);

#pragma thumb on
void func_ov001_022e19bc(s32 unused, s32 a, s32 b) {
    if (a >= 7) {
        b = 2;
    }
    func_0205247c(a, b);
}
#pragma thumb reset
