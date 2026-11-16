#include <nitro/types.h>

typedef struct UnkNode_0203d49c {
    struct UnkNode_0203d49c *next;
    u8 pad4[0x78];
    s32 type;
    u32 unk80;
    u8 pad84[8];
    u32 a;
    u32 b;
} UnkNode_0203d49c;

typedef struct {
    u8 pad[0x8408];
    UnkNode_0203d49c *head;
} UnkStruct_0203d49c;

UnkNode_0203d49c *WFSi_FromFreeToBusy(UnkStruct_0203d49c *w);
void WFSi_NotifyBusy(void);

UnkNode_0203d49c *WFSi_FindBusy(UnkStruct_0203d49c *w, u32 a, u32 b) {
    UnkNode_0203d49c *n;
    for (n = w->head; n != NULL; n = n->next) {
        if (n->type == 1 && n->a == a && n->b == b)
            break;
    }
    if (n == NULL) {
        n = WFSi_FromFreeToBusy(w);
        n->unk80 = 0;
        n->a = a;
        n->b = b;
        WFSi_NotifyBusy();
    }
    return n;
}
