/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov001_022fa558();

void func_ov001_022fc318(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x84);
    func_ov001_022fa558();
    s32 t1 = (*(u8 *)((u8 *)t0 + 0x10c) & 2);
    if (t1 != 0) {
        *(u32 *)((u8 *)p0 + 0x14) = (*(u32 *)((u8 *)p0 + 0x14) | 8);
    }
}
