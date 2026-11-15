/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_ov001_022ebe50();
s32 func_ov001_022ebe4c();
s32 func_ov001_022ebdd8();
s32 func_ov001_022e9804();
s32 func_02037d40();
extern u8 data_022503e0[];
s32 func_020351d8();

void func_ov001_022ebe98(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x10);
    func_ov001_022ebe50(*(u32 *)((u8 *)p0 + 0x10));
    func_ov001_022ebe4c(t0);
    s32 t1 = func_ov001_022ebdd8(t0);
    s32 t2 = func_ov001_022e9804();
    func_02037d40();
    s32 t3 = func_020351d8(data_022503e0, 0, 0);
}

#pragma thumb reset
