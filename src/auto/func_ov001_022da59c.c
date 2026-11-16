/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_ov001_022d9e50();
s32 func_ov001_022d9e2c();
s32 func_ov001_022d9d98();
s32 func_ov001_022d9d64();
s32 func_ov001_022d9cf8();
s32 func_ov001_022d9ab4();

void func_ov001_022da59c(s32 p0) {
    func_ov001_022d9e50(p0, 0x38, 0x98);
    func_ov001_022d9e2c(p0, 0x10, 0x48);
    func_ov001_022d9d98(p0, 0, 0, 0);
    func_ov001_022d9d98(p0, 1, 0, 0);
    func_ov001_022d9d64(p0, 0, 0x58, 0x44);
    func_ov001_022d9d64(p0, 1, 0x58, 0x74);
    func_ov001_022d9cf8(p0, 0, 0x68, 0x43);
    func_ov001_022d9cf8(p0, 1, 0x68, 0x73);
    func_ov001_022d9ab4(p0, 0xb0, 8, 0, 0);
}

#pragma thumb reset
