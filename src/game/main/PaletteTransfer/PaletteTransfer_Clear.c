#include <nitro/types.h>

typedef struct UnkNode_02035368 UnkNode_02035368;
struct UnkNode_02035368 { UnkNode_02035368 *next; };
extern UnkNode_02035368 *gPaletteTransferHead;
extern UnkNode_02035368 *gPaletteTransferTail;
void TransferJob_Free(void *p);

void PaletteTransfer_Clear(void) {
    UnkNode_02035368 *n = gPaletteTransferHead;
    while (n != NULL) {
        TransferJob_Free(n);
        n = gPaletteTransferHead->next;
        if (n == NULL) gPaletteTransferTail = NULL;
        gPaletteTransferHead = n;
    }
}
