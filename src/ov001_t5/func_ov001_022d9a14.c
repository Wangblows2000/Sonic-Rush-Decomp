#include <nitro/types.h>

extern s32 gPaletteTransferHead;
extern s32 gPaletteTransferTail;

void PaletteTransfer_QueueCopy(u16 *src, s32 n, s32 a, u32 dst);
void PaletteTransfer_Flush(void);

#pragma thumb on
void func_ov001_022d9a14(void *p, u16 col) {
    s32 s0 = gPaletteTransferHead;
    s32 s1 = gPaletteTransferTail;
    gPaletteTransferHead = 0;
    gPaletteTransferTail = 0;
    PaletteTransfer_QueueCopy(&col, 1, 0, 0x500079e);
    PaletteTransfer_Flush();
    gPaletteTransferHead = s0;
    gPaletteTransferTail = s1;
}
#pragma thumb reset
