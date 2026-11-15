/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov000_0236ac60[];
s32 func_ov000_022f4a84();

void func_ov000_022f4954(s32 p0) {
    s32 t0 = func_ov000_022f4a84(p0, *(u32 *)((u8 *)*(u32 *)data_ov000_0236ac60 + 0x10), 0x4000, 0x200, 0x60, 0x80, 0x700);
}
