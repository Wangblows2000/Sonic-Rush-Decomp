/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_02316b5c();

void func_ov000_02316a1c(u8 *p0) {
    if ((*(u16 *)((u8 *)p0 + 0x5c) & 8) == 0) {
        return;
    }
    s32 t0 = (*(u16 *)((u8 *)p0 + 0x13e) & ~0x100);
    *(u16 *)((u8 *)p0 + 0x13e) = t0;
    func_ov000_02316b5c(p0, ((u8 *)p0 + 0x100), t0);
}
