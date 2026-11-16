/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 data_022b4574[];
s32 func_ov000_023611f8();
s32 func_ov000_0236119c();
s32 func_ov000_02360998();
s32 func_ov000_0235fb7c();
s32 func_02051628();
s32 func_ov000_0235f850();
void func_ov000_02361478();

void func_ov000_02361614(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10);
    func_ov000_023611f8(*(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10));
    func_ov000_0236119c(t0, 0x1000, 0);
    func_ov000_02360998(t0);
    *(u32 *)t0 = (*(u32 *)t0 + 0x1);
    s32 t1 = func_ov000_0235fb7c(t0);
    if (t1 != 0) {
        *(u32 *)t0 = 0;
        func_02051628(0);
        func_ov000_0235f850(0, 0x10, 0x10, 0);
        *(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x8) = (u32)func_ov000_02361478;
    }
}

#pragma thumb reset
