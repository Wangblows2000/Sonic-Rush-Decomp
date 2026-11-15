/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

void func_ov001_022d0094();
s32 func_020372d0();

s32 func_ov001_022d0158(s32 p0) {
    s32 t0 = func_020372d0((u32)func_ov001_022d0094, 0, 0, 0x5000, 0x1173, 0xc);
    s32 t1 = *(u32 *)((u8 *)t0 + 0x10);
    *(u32 *)t1 = p0;
    *(u32 *)((u8 *)t1 + 0x4) = 0x80;
    *(u32 *)((u8 *)t1 + 0x8) = 0x1d0;
    return t0;
}

#pragma thumb reset
