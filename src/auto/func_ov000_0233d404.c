/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_02007318();

void func_ov000_0233d404(s32 p0, s32 p1) {
    func_02007318(((s32)((u8 *)p0 + 0xc) + (p1 * 0x28)), p1, 0x28);
    func_02007318(((s32)((u8 *)p0 + 0x5c) + (p1 * 0x28)));
}
