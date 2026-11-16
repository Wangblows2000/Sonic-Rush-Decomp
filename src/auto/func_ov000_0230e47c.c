/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_0230e868();
s32 func_ov000_0234fba0();

s32 func_ov000_0230e47c(u8 *p0, u8 *p1) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x18);
    s32 t1 = *(u32 *)((u8 *)p1 + 0x18);
    if (t0 == 0) {
        return (s32)p0;
    }
    if (t1 == 0) {
        s32 t2 = *(u8 *)t1;
        if (t2 != 1) {
            return t2;
        }
    }
    s32 t3 = *(u32 *)((u8 *)t0 + 0x10);
    s32 t4 = *(u32 *)((u8 *)t1 + 0x10);
    s32 t5 = (*(u32 *)((u8 *)t0 + 0x10) - 0xb00);
    if (t5 >= t4) {
        return t5;
    }
    if ((t3 - 0x80) <= t4) {
        return (t3 - 0x80);
    }
    *(u32 *)((u8 *)t0 + 0x204) = t1;
    func_ov000_0230e868(t0, t3, t4);
    s32 t6 = func_ov000_0234fba0(t1, t0);
    return t6;
}
