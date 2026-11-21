#include <nitro/types.h>

typedef struct UnkNode_02035c58 UnkNode_02035c58;
struct UnkNode_02035c58 { UnkNode_02035c58 *next; };
extern UnkNode_02035c58 *gScreenTransferHead;
extern UnkNode_02035c58 *gScreenTransferTail;
UnkNode_02035c58 *TransferJob_Alloc(void);
UnkNode_02035c58 *OS_GetArenaLo(u32 a);

UnkNode_02035c58 *ScreenTransfer_AllocJob(void) {
    UnkNode_02035c58 *n = TransferJob_Alloc();
    if (n != OS_GetArenaLo(0)) {
        if (gScreenTransferHead == NULL) {
            gScreenTransferHead = n;
        } else {
            gScreenTransferTail->next = n;
        }
        gScreenTransferTail = n;
        n->next = NULL;
    }
    return n;
}
