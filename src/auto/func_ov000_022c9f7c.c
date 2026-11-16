/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_022c9dbc();

void func_ov000_022c9f7c(s32 p0, s32 p1) {
    *(u16 *)((u8 *)p0 + 0x21a) = 0;
    func_ov000_022c9dbc(p0, p1, 0xc00, 0x180, 0x60, 0x500, 0x500);
}
