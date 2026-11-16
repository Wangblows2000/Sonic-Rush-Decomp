#include <nitro/types.h>

typedef struct UnkNode_0203530c UnkNode_0203530c;
struct UnkNode_0203530c { UnkNode_0203530c *next; };
extern UnkNode_0203530c *gPaletteTransferHead;
extern UnkNode_0203530c *gPaletteTransferTail;
UnkNode_0203530c *TransferJob_Alloc(void);
UnkNode_0203530c *OS_GetArenaLo(u32 a);

UnkNode_0203530c *PaletteTransfer_AllocJob(void) {
    UnkNode_0203530c *n = TransferJob_Alloc();
    if (n != OS_GetArenaLo(0)) {
        if (gPaletteTransferHead == NULL) {
            gPaletteTransferHead = n;
        } else {
            gPaletteTransferTail->next = n;
        }
        gPaletteTransferTail = n;
        n->next = NULL;
    }
    return n;
}
