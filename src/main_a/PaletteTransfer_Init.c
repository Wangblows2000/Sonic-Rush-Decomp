#include <nitro/types.h>

extern u32 gPaletteTransferHead;
extern u32 gPaletteTransferTail;

void PaletteTransfer_Init(void) {
    gPaletteTransferHead = 0;
    gPaletteTransferTail = 0;
}
