/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

void func_ov000_0235e23c();
s32 func_020372d0();

void func_ov000_0235e31c(void) {
    func_020372d0((u32)func_ov000_0235e23c, 0, 0, 0x1000, 0x1000, 0);
}

#pragma thumb reset
