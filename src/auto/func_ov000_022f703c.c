/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_02031768();
s32 func_0201bf1c();
s32 func_ov000_022fb9c8();

void func_ov000_022f703c(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x10);
    func_02031768((*(u32 *)((u8 *)p0 + 0x10) + 0x4d4));
    func_0201bf1c((t0 + 0x4a8));
    s32 t1 = func_ov000_022fb9c8(p0);
}
