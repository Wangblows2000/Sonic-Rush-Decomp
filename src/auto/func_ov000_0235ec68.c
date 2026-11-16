/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_02037d40();
s32 func_ov000_0235d158();
s32 func_ov000_0235d958();
s32 func_ov000_0235ec64();
s32 func_ov000_0235ec2c();
s32 func_ov000_0235ebb8();
s32 func_ov000_0235c6a8();

void func_ov000_0235ec68(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x10);
    func_02037d40();
    func_ov000_0235d158(t0);
    func_ov000_0235d958(t0);
    func_ov000_0235ec64(t0);
    func_ov000_0235ec2c(t0);
    func_ov000_0235ebb8(t0);
    func_ov000_0235c6a8();
}

#pragma thumb reset
