/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_02039c18();
s32 func_0203363c();
s32 func_02033550();

void func_ov001_022ce670(s32 p0, u8 *p1, s32 p2) {
    s32 t0 = func_02039c18(1, *(u16 *)((u8 *)((s32)p1 + *(u32 *)((u8 *)p1 + 0x18)) + 0xa));
    func_0203363c(p0, p1, p2, 0, 1, 0, t0, 0, 0x5000600);
    s32 t1 = func_02033550(p0, p2);
}

#pragma thumb reset
