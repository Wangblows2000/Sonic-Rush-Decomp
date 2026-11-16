/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gPlayers[];
s32 func_ov000_022f4a84();

void func_ov000_022f4954(s32 p0) {
    func_ov000_022f4a84(p0, *(u32 *)((u8 *)*(u32 *)gPlayers + 0x10), 0x4000, 0x200, 0x60, 0x80, 0x700);
}
