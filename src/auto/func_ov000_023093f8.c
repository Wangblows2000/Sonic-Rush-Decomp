/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_0234237c();
s32 func_ov000_0234232c();
s32 func_ov000_023093e8();

void func_ov000_023093f8(u8 *p0) {
    s32 t0 = func_ov000_0234237c(1, *(u8 *)((u8 *)p0 + 0x214));
    if (t0 == 0) {
        return;
    }
    func_ov000_0234232c(*(u8 *)((u8 *)p0 + 0x214));
    func_ov000_023093e8(p0);
}
