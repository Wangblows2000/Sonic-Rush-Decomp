/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_02042958();
s32 func_02042998();

s32 func_02050c60(void) {
    s32 t0 = func_02042958();
    if (t0 < 2) {
        return 0;
    }
    s32 t1 = func_02042998(t0);
    return (t1 - 0x1);
}

#pragma thumb reset
