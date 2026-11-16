/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_02039c18();
s32 func_0203363c();

void func_ov001_022e9f04(u8 *p0, u8 *p1, s32 p2, s32 p3) {
    s32 t0 = func_02039c18(1, *(u16 *)((u8 *)((s32)p1 + *(u32 *)((u8 *)p1 + 0x18)) + 0xa));
    func_0203363c(p0, p1, p2, 0, 1, 0, t0, 0, 0x5000600);
    *(u32 *)((u8 *)p0 + 0x2c) = (*(u32 *)((u8 *)p0 + 0x2c) & 0xfff9ffff);
    *(u32 *)((u8 *)p0 + 0x2c) = (*(u32 *)((u8 *)p0 + 0x2c) | ((s32)p3 << 0x11));
}

#pragma thumb reset
