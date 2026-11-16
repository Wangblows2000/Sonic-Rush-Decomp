#include <nitro/types.h>

extern u32 gScreenTransferHead;
extern u32 gScreenTransferTail;

void ScreenTransfer_Init(void) {
    gScreenTransferHead = 0;
    gScreenTransferTail = 0;
}
