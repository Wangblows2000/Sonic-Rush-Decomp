/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_022f685c();
s32 func_ov000_022f6a80();
void func_ov000_022f63cc();

void func_ov000_022f6500(u8 *p0) {
    func_ov000_022f685c();
    s32 t0 = (*(u16 *)((u8 *)p0 + 0x210) + 0x800);
    *(u16 *)((u8 *)p0 + 0x210) = t0;
    s32 t1 = func_ov000_022f6a80(p0, ((u8 *)p0 + 0x200), t0);
    if (t1 != 0) {
        *(u32 *)((u8 *)p0 + 0x68) = (u32)func_ov000_022f63cc;
    }
}
