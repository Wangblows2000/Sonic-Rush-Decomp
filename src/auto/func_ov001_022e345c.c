/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_0203344c();
s32 func_02032d2c();

void func_ov001_022e345c(s32 p0) {
    s32 t0 = (*(u32 *)((u8 *)p0 + 0xa4) + 0x1b08);
    *(u16 *)((u8 *)t0 + 0x130) = 8;
    *(u16 *)((u8 *)t0 + 0x132) = 0x98;
    func_0203344c((t0 + 0x12c), 0, 0);
    func_02032d2c((t0 + 0x12c));
}

#pragma thumb reset
