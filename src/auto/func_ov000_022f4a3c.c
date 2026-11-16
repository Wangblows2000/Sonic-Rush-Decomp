/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov000_0236ac60[];
s32 func_ov000_022f4a84();

void func_ov000_022f4a3c(s32 p0) {
    func_ov000_022f4a84(p0, *(u32 *)((u8 *)*(u32 *)data_ov000_0236ac60 + 0x10), 0x4000, 0x100, 0x30, 0x40, 0x700);
}
