/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_0203344c();
s32 func_02032d2c();

void func_ov001_022d9d64(s32 p0, s32 p1, s32 p2, s32 p3) {
    *(u16 *)((u8 *)((s32)((u8 *)p0 + 0x210) + (0x48 * (s32)((u8 *)p1 + 0x2))) + 0x4) = p2;
    *(u16 *)((u8 *)((s32)((u8 *)p0 + 0x210) + (0x48 * (s32)((u8 *)p1 + 0x2))) + 0x6) = p3;
    *(u16 *)(((s32)((u8 *)p0 + 0x210) + (0x48 * (s32)((u8 *)p1 + 0x2))) + 0x46) = 8;
    func_0203344c(((s32)((u8 *)p0 + 0x210) + (0x48 * (s32)((u8 *)p1 + 0x2))), 0, 0);
    s32 t0 = func_02032d2c(((s32)((u8 *)p0 + 0x210) + (0x48 * (s32)((u8 *)p1 + 0x2))));
}

#pragma thumb reset
