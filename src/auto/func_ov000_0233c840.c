/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 PaletteTransfer_QueueCopy();

void func_ov000_0233c840(s32 p0) {
    PaletteTransfer_QueueCopy(p0, 0xf0, 0, 0x5000000);
    PaletteTransfer_QueueCopy(p0, 0xf0, 0, 0x5000400);
}
