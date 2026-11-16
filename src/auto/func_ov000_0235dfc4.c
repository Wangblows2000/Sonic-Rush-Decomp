/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_ov000_0235d6f8();
s32 func_ov000_0235d5a0();
s32 func_ov000_0235d448();
s32 func_ov000_0235d6f4();

void func_ov000_0235dfc4(u8 *p0) {
    func_ov000_0235d6f8();
    if (*(u32 *)((u8 *)p0 + 0x18) == 0) {
        func_ov000_0235d5a0(p0, 0x38, 0x90, 1);
    } else {
        func_ov000_0235d448(p0, 0x38, 0x90, 1);
    }
    if (*(u32 *)((u8 *)p0 + 0x1c) == 0) {
        func_ov000_0235d5a0(p0, 0xc8, 0x90, 0);
    } else {
        func_ov000_0235d448(p0, 0xc8, 0x90, 0);
    }
    func_ov000_0235d6f4(p0);
}

#pragma thumb reset
