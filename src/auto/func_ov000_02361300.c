/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 data_022b4574[];
s32 func_02042958();
s32 func_02054dbc();
s32 func_020514ec();
s32 func_020371c8();

void func_ov000_02361300(void) {
    s32 t0 = *(u32 *)*(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10);
    s32 t1 = func_02042958(*(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10));
    if (t1 == 7) {
        func_02054dbc();
        return;
    }
    s32 t2 = func_020514ec();
    if (t2 != 0) {
        *(u32 *)((u8 *)t0 + 0x20) = 1;
        func_020371c8(*(u32 *)data_022b4574);
    }
}

#pragma thumb reset
