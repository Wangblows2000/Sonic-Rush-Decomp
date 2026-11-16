/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_ov001_022d94b4();
s32 func_ov001_022d70ec();
s32 func_ov001_022d7210();
s32 func_ov001_022d79d8();
s32 func_ov001_022d74dc();

void func_ov001_022d9530(void) {
    s32 t0 = func_ov001_022d94b4();
    if (*(u32 *)((u8 *)t0 + 0x8) != 0) {
        func_ov001_022d70ec(t0, 1, 1);
    } else {
        func_ov001_022d7210(t0, 0, 0);
    }
    func_ov001_022d79d8(8, 0);
    func_ov001_022d74dc(t0, 0);
    *(volatile u32 *)0x4000000 = ((*(volatile u32 *)0x4000000 & -7937) | 0x1f00);
    *(volatile u32 *)0x4001000 = ((*(volatile u32 *)0x4001000 & -7937) | 0x1f00);
}

#pragma thumb reset
