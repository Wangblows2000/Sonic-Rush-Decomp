/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gPlayers[];
s32 func_ov000_022f4a84();

void func_ov000_022f490c(s32 p0) {
    func_ov000_022f4a84(p0, *(u32 *)((u8 *)*(u32 *)gPlayers + 0x10), 0x2000, 0x100, 0x20, 0x40, 0x700);
}
