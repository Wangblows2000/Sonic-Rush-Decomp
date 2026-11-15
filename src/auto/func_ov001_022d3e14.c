/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_02037d40();
extern u8 data_02087cb8[];
s32 func_ov001_022d3e10();
s32 func_ov001_022d3dd8();
s32 func_ov001_022d3d64();
s32 func_ov001_022d3044();

void func_ov001_022d3e14(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x10);
    func_02037d40();
    s32 t1 = (*(u32 *)data_02087cb8 & -1025);
    *(u32 *)data_02087cb8 = t1;
    func_ov001_022d3e10(t0, data_02087cb8, t1);
    func_ov001_022d3dd8(t0);
    s32 t2 = func_ov001_022d3d64(t0);
    s32 t3 = func_ov001_022d3044();
}

#pragma thumb reset
