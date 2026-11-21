#include <nitro/types.h>

extern u32 gVRAMTransferHead;
extern u32 gVRAMTransferTail;
void VRAMTransfer_QueueLoad(u32 a, u32 b, u32 c);
void VRAMTransfer_Flush(void);

void VRAMTransfer_LoadNow(u32 a, u32 b, u32 c) {
    u32 head = gVRAMTransferHead;
    u32 tail = gVRAMTransferTail;
    gVRAMTransferHead = 0;
    gVRAMTransferTail = 0;
    VRAMTransfer_QueueLoad(a, b, c);
    VRAMTransfer_Flush();
    gVRAMTransferHead = head;
    gVRAMTransferTail = tail;
}
