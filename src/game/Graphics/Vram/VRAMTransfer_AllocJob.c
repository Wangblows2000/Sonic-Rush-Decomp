#include <nitro/types.h>

typedef struct UnkNode_020336f8 UnkNode_020336f8;
struct UnkNode_020336f8 { UnkNode_020336f8 *next; };
extern UnkNode_020336f8 *gVRAMTransferHead;
extern UnkNode_020336f8 *gVRAMTransferTail;
UnkNode_020336f8 *TransferJob_Alloc(void);
UnkNode_020336f8 *OS_GetArenaLo(u32 a);

UnkNode_020336f8 *VRAMTransfer_AllocJob(void) {
    UnkNode_020336f8 *n = TransferJob_Alloc();
    if (n != OS_GetArenaLo(0)) {
        if (gVRAMTransferHead == NULL) {
            gVRAMTransferHead = n;
        } else {
            gVRAMTransferTail->next = n;
        }
        gVRAMTransferTail = n;
        n->next = NULL;
    }
    return n;
}
