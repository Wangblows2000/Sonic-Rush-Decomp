#include <nitro/types.h>

typedef struct UnkNode_02033754 UnkNode_02033754;
struct UnkNode_02033754 { UnkNode_02033754 *next; };
extern UnkNode_02033754 *gVRAMTransferHead;
extern UnkNode_02033754 *gVRAMTransferTail;
void TransferJob_Free(void *p);

void VRAMTransfer_Clear(void) {
    UnkNode_02033754 *n = gVRAMTransferHead;
    while (n != NULL) {
        TransferJob_Free(n);
        n = gVRAMTransferHead->next;
        if (n == NULL) gVRAMTransferTail = NULL;
        gVRAMTransferHead = n;
    }
}
