/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_02063750();
s32 func_02048594();

void func_020485f0(s32 p0) {
    s32 t0 = func_02063750();
    if (t0 == 0) {
        func_02048594(p0);
    }
}

#pragma thumb reset
