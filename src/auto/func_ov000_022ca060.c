/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov000_0236ac60[];
s32 func_ov000_022c9dbc();

void func_ov000_022ca060(s32 p0) {
    s32 t0 = func_ov000_022c9dbc(p0, *(u32 *)((u8 *)*(u32 *)data_ov000_0236ac60 + 0x10), 0x2000, 0x100, 0x20, 0x40, 0x400);
}
