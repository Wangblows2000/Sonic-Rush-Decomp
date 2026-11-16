#include <nitro/types.h>
#include <main_sdk2/mbnode.h>

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 e);

UnkMbNode *WBT_UnregisterBlock(u32 id)
{
    UnkMbNode *n;
    UnkMbNode *prev;
    u32 e = OS_DisableInterrupts();
    prev = WBT_Unk_0201558c();
    if (prev == NULL) {
        OS_RestoreInterrupts(e);
        return NULL;
    }
    if (prev->id == id) {
        WBT_Unk_0201557c(prev->next);
        OS_RestoreInterrupts(e);
        return prev;
    }
    for (n = prev->next; n != NULL; n = n->next) {
        if (n->id == id) {
            prev->next = n->next;
            OS_RestoreInterrupts(e);
            return n;
        }
        prev = n;
    }
    OS_RestoreInterrupts(e);
    return NULL;
}
