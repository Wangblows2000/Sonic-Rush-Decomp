/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_02042958();
s32 func_02054dbc();
extern u8 data_0207ad9c[];
s32 func_02051628();
s32 func_02051384();

s32 func_020514ec(void) {
    s32 t0 = func_02042958();
    if (t0 == 7) {
        func_02054dbc();
        return 0;
    }
    s32 t1 = func_02042958(t0);
    if (t1 != 6) {
        return 0;
    }
    if (*(u16 *)data_0207ad9c == 0) {
        func_02051628(1);
        func_02051384();
    } else {
        func_02051628(0);
    }
    return 1;
}

#pragma thumb reset
