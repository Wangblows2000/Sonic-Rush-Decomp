/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 data_022b4574[];
s32 func_02063030();
s32 func_02042958();
s32 func_020371c8();
s32 func_02054dbc();
s32 func_020514ec();
s32 func_02051384();
extern u8 data_022c4500[];
s32 func_0204319c();
s32 func_0204331c();
void func_0206340c();

void func_02063480(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10);
    func_02063030(*(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10));
    s32 t1 = func_02042958();
    if (t1 == 7) {
        func_020371c8(*(u32 *)data_022b4574);
        func_02054dbc();
        return;
    }
    s32 t2 = func_020514ec();
    if (t2 != 0) {
        func_02051384();
        func_0204319c(data_022c4500);
        func_0204331c(1);
        *(u32 *)((u8 *)t0 + 0xc) = 0;
        *(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x8) = (u32)func_0206340c;
    }
}

#pragma thumb reset
