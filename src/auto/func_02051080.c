/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_02051838();
s32 func_0206cd7c();
s32 func_02050a7c();

void func_02051080(s32 p0) {
    func_02051838();
    func_0206cd7c(0);
    s32 t0 = func_02050a7c(p0);
}

#pragma thumb reset
