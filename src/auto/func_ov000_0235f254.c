/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_ov000_0235cc60();
s32 func_ov000_0235cc4c();
s32 func_ov000_0235cbf0();
s32 func_ov000_0235f17c();
s32 func_ov000_0235c6d8();
s32 func_020513a8();
s32 func_0204b528();
s32 func_0204b624();
s32 func_ov000_0235f024();
s32 func_ov000_0235ee18();
s32 func_ov000_0235ed5c();
s32 func_ov000_0235dab4();
s32 func_ov000_0235d888();
s32 func_ov000_0235d7b4();
s32 func_ov000_0235d1a4();
s32 func_ov000_0235cd58();
s32 func_02037d60();

void func_ov000_0235f254(void) {
    s32 t0 = func_ov000_0235cc60();
    s32 t1 = func_ov000_0235cc4c();
    s32 t2 = func_ov000_0235cbf0();
    s32 t3 = func_ov000_0235f17c(t0, t1, t2);
    func_ov000_0235c6d8();
    s32 t4 = func_020513a8();
    if (t4 == 0) {
        func_0204b528();
        func_0204b624();
    }
    func_ov000_0235f024(t3);
    func_ov000_0235ee18(t3);
    func_ov000_0235ed5c(t3);
    func_ov000_0235dab4(t3);
    func_ov000_0235d888(t3, 0);
    func_ov000_0235d7b4(t3, 0);
    func_ov000_0235d1a4(t3);
    func_ov000_0235cd58((t3 + 0x4ec));
    func_02037d60();
}

#pragma thumb reset
