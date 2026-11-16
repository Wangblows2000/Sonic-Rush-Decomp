/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_02039c18();
s32 func_0203363c();

void func_ov001_022dd8d8(u8 *p0, u8 *p1, s32 p2) {
    s32 v0 = (s32)p1;
    s32 v1 = (s32)p2;
    if ((u32)(s32)p2 >= (u32)((u32)((*(u32 *)((u8 *)p1 + *(u32 *)((u8 *)p1 + 0x4)) - 0x4) << 0xd) >> 0x10)) {
        v0 = *(u16 *)((u8 *)((s32)p1 + *(u32 *)((u8 *)p1 + 0x18)) + 0xa);
    } else {
        v0 = *(u16 *)((u8 *)(((s32)p1 + *(u32 *)((u8 *)p1 + 0x18)) + (0x14 * (s32)p2)) + 0x20);
        v1 = ((s32)p1 + *(u32 *)((u8 *)p1 + 0x18));
    }
    s32 t2 = func_02039c18(0, v0, v1);
    func_0203363c(p0, p1, p2, 0, 0, 0, t2, 0, 0x5000200);
    *(u32 *)((u8 *)p0 + 0x2c) = (*(u32 *)((u8 *)p0 + 0x2c) & 0xfff9ffff);
}

#pragma thumb reset
