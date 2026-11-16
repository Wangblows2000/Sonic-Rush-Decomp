/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 data_022b4574[];
s32 func_02063030();
s32 func_02042958();
s32 func_020371c8();
s32 func_02054dbc();
s32 func_020514ec();
s32 func_0204b718();
s32 func_0204b700();
void func_02063588();

void func_020635d0(void) {
    func_02063030(*(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10));
    s32 t0 = func_02042958();
    if (t0 == 7) {
        func_020371c8(*(u32 *)data_022b4574);
        func_02054dbc();
        return;
    }
    s32 t1 = func_020514ec();
    if (t1 != 0) {
        func_0204b718(1);
        func_0204b700();
        *(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x8) = (u32)func_02063588;
    }
}

#pragma thumb reset
