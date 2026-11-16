/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 data_022b4574[];
s32 func_02063030();
s32 func_0204b700();
s32 func_020371c8();
s32 func_02054dbc();
s32 func_020516b4();
void func_0206353c();

void func_02063588(void) {
    func_02063030(*(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10));
    s32 t0 = func_0204b700();
    if (t0 == 2) {
        func_020371c8(*(u32 *)data_022b4574);
        func_02054dbc();
        return;
    }
    if (t0 == 1) {
        func_020516b4();
        *(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x8) = (u32)func_0206353c;
    }
}

#pragma thumb reset
