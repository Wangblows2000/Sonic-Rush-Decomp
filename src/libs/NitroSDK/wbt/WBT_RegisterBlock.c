#include <nitro/types.h>
#include <NitroSDK/mbnode.h>

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 e);
void MI_CpuCopy8(const void *src, void *dest, u32 size);
void MI_CpuFill8(void *dest, u8 data, u32 size);

BOOL WBT_RegisterBlock(UnkMbNode *node, u32 id, const void *name, u8 *data, u32 size, u16 x)
{
    u32 e = OS_DisableInterrupts();
    if (WBT_GetBlockListHead() == NULL) {
        WBT_SetBlockListHead(node);
    } else {
        UnkMbNode *n = WBT_GetBlockListHead();
        while (n->next != NULL) {
            n = n->next;
        }
        n->next = node;
    }
    if (data == NULL) {
        node->unk32 = 1;
    } else {
        node->data = data;
        node->unk32 = 0;
    }
    node->next = NULL;
    node->id = id;
    node->size = size;
    node->unk30 = x;
    if (name != NULL) {
        MI_CpuCopy8(name, node->name, 0x20);
    } else {
        MI_CpuFill8(node->name, 0, 0x20);
    }
    OS_RestoreInterrupts(e);
    return TRUE;
}
