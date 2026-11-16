/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_0231c21c();

void func_ov000_0231c36c(u8 *p0, u8 *p1) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x18);
    s32 t1 = *(u32 *)((u8 *)p1 + 0x18);
    if (t0 == 0) {
        return;
    }
    if (t1 == 0) {
        return;
    }
    func_ov000_0231c21c(t0, t1);
}
