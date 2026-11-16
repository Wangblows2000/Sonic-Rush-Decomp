/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_02350968();

void func_ov000_0231630c(u8 *p0, u8 *p1) {
    u32 t0 = *(u32 *)((u8 *)p0 + 0x18);
    u32 t1 = *(u32 *)((u8 *)p1 + 0x18);
    if (t0 == 0) {
        return;
    }
    if (t1 == 0) {
        u8 t2 = *(u8 *)t1;
        if (t2 != 1) {
            return;
        }
    }
    *(u32 *)((u8 *)t0 + 0x204) = t1;
    u32 t3 = *(u32 *)((u8 *)t0 + 0x54);
    *(u32 *)((u8 *)t0 + 0x54) = (t3 | 2);
    func_ov000_02350968(t1, t0, (t3 | 2), t0);
}
