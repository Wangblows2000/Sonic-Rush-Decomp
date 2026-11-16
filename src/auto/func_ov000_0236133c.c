/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_02042958();
s32 func_02054dbc();
s32 func_02051660();
s32 func_02051598();
void func_ov000_02361300();
extern u8 data_022b4574[];

void func_ov000_0236133c(void) {
    s32 t0 = func_02042958();
    if (t0 == 7) {
        func_02054dbc();
        return;
    }
    s32 t1 = func_02051660();
    if (t1 != 0) {
        func_02051598(0x40);
        *(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x8) = (u32)func_ov000_02361300;
    }
}

#pragma thumb reset
