#include <nitro/types.h>

extern u8 *gSpecialStageHud;
extern s32 gPaletteTransferHead;
extern s32 gPaletteTransferTail;
void PaletteTransfer_QueueCopy(void *p, s32 a, s32 b, u32 c);
void PaletteTransfer_Flush(void);

void func_ov001_0230956c(void) {
    s32 a = gPaletteTransferHead;
    s32 b = gPaletteTransferTail;
    gPaletteTransferHead = 0;
    gPaletteTransferTail = 0;
    PaletteTransfer_QueueCopy(gSpecialStageHud + 0xb88, 0x10, 0, 0x05000700);
    PaletteTransfer_Flush();
    gPaletteTransferHead = a;
    gPaletteTransferTail = b;
}
