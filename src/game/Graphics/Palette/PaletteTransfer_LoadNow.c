#include <nitro/types.h>

extern u32 gPaletteTransferHead;
extern u32 gPaletteTransferTail;
void PaletteTransfer_QueueLoad(u32 a, u32 b, u32 c);
void PaletteTransfer_Flush(void);

void PaletteTransfer_LoadNow(u32 a, u32 b, u32 c) {
    u32 head = gPaletteTransferHead;
    u32 tail = gPaletteTransferTail;
    gPaletteTransferHead = 0;
    gPaletteTransferTail = 0;
    PaletteTransfer_QueueLoad(a, b, c);
    PaletteTransfer_Flush();
    gPaletteTransferHead = head;
    gPaletteTransferTail = tail;
}
