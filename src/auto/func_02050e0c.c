/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_02051628();
s32 func_02051838();
s32 func_0206cd7c();
s32 func_020509a0();

void func_02050e0c(s32 p0) {
    func_02051628(0);
    func_02051838();
    func_0206cd7c(0);
    func_020509a0(p0);
}

#pragma thumb reset
