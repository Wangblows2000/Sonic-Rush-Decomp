#include <nitro/types.h>

extern u32 gVRAMTransferHead;
extern u32 gVRAMTransferTail;

void VRAMTransfer_Init(void) {
    gVRAMTransferHead = 0;
    gVRAMTransferTail = 0;
}
