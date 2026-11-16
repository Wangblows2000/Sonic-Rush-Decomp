/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_02069818();
void func_ov000_0230f8c8();

void func_ov000_0230f9cc(u8 *p0) {
    s32 t0 = *(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0x1e0) + 0x2);
    if (t0 == 0x53) {
        func_02069818(p0, 2);
    } else {
        if (t0 == 0x52) {
            func_02069818(p0, 1);
        } else {
            func_02069818(p0, 0);
        }
    }
    *(u32 *)((u8 *)p0 + 0x68) = (u32)func_ov000_0230f8c8;
}
