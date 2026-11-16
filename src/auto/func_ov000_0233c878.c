/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 PaletteTransfer_QueueCopy();

void func_ov000_0233c878(s32 p0) {
    PaletteTransfer_QueueCopy(p0, 0xf0, 1, 0x4000);
    PaletteTransfer_QueueCopy(p0, 0xf0, 1, 0x6000);
    PaletteTransfer_QueueCopy(p0, 0xf0, 3, 0x4000);
    PaletteTransfer_QueueCopy(p0, 0xf0, 3, 0x6000);
}
