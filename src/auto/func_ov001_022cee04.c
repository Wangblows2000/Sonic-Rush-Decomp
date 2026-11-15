/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_ov001_022cecb8();
s32 func_ov001_022cdfdc();

void func_ov001_022cee04(void) {
    s32 t0 = func_ov001_022cecb8();
    func_ov001_022cdfdc(t0, 1, 0);
    *(volatile u32 *)0x4000000 = ((*(volatile u32 *)0x4000000 & -7937) | 0x1c00);
    *(volatile u32 *)0x4001000 = ((*(volatile u32 *)0x4001000 & -7937) | 0x1900);
}

#pragma thumb reset
