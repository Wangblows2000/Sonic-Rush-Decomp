/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_02037d40();
s32 func_02035368();
s32 func_02035cb4();
s32 func_02033754();
s32 func_ov001_022d6738();
s32 func_ov001_022d6734();
s32 func_ov001_022d66f8();
s32 func_ov001_022d6684();
s32 func_ov001_022d4ff0();

void func_ov001_022d682c(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x10);
    s32 t1 = func_02037d40();
    s32 t2 = func_02035368();
    s32 t3 = func_02035cb4();
    func_02033754();
    func_ov001_022d6738(t0);
    func_ov001_022d6734(t0);
    func_ov001_022d66f8(t0);
    s32 t4 = func_ov001_022d6684(t0);
    s32 t5 = func_ov001_022d4ff0();
}

#pragma thumb reset
