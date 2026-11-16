/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_02307e84();
s32 func_ov000_0234237c();
s32 func_ov000_02307dfc();
s32 func_ov000_02308dac();

void func_ov000_02308df8(u8 *p0) {
    s32 t0 = func_ov000_02307e84();
    s32 t1 = func_ov000_0234237c(1, t0);
    s32 t2 = func_ov000_02307dfc(t1, 0);
    if (t2 != 0) {
        *(u32 *)((u8 *)p0 + 0x1f4) = (*(u32 *)((u8 *)p0 + 0x1f4) & ~0x10);
        *(u32 *)((u8 *)p0 + 0x54) = (*(u32 *)((u8 *)p0 + 0x54) | 4);
        return;
    }
    if (t1 == 0) {
        return;
    }
    s32 t3 = *(u32 *)((u8 *)t1 + 0x20c);
    if ((*(u32 *)((u8 *)t1 + 0x20c) & 0x40) != 0) {
        return;
    }
    if ((t3 & 0x10) == 0) {
        return;
    }
    func_ov000_02308dac(p0, t3);
}
