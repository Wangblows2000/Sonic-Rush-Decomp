#include <nitro/types.h>

typedef struct UnkNode_02035cb4 UnkNode_02035cb4;
struct UnkNode_02035cb4 { UnkNode_02035cb4 *next; };
extern UnkNode_02035cb4 *gScreenTransferHead;
extern UnkNode_02035cb4 *gScreenTransferTail;
void TransferJob_Free(void *p);

void ScreenTransfer_Clear(void) {
    UnkNode_02035cb4 *n = gScreenTransferHead;
    while (n != NULL) {
        TransferJob_Free(n);
        n = gScreenTransferHead->next;
        if (n == NULL) gScreenTransferTail = NULL;
        gScreenTransferHead = n;
    }
}
