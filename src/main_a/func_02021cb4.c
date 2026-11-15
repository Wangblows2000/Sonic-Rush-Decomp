#include <nitro/types.h>

typedef struct UnkNode_02021cb4 UnkNode_02021cb4;
struct UnkNode_02021cb4 { u8 unk0[0x10]; UnkNode_02021cb4 *next; };

BOOL func_02021cb4(UnkNode_02021cb4 **head, UnkNode_02021cb4 *node) {
    UnkNode_02021cb4 *cur;
    UnkNode_02021cb4 *prev = *head;
    if (prev == NULL) return FALSE;
    if (prev == node) {
        *head = prev->next;
        node->next = NULL;
        return TRUE;
    }
    for (cur = prev->next; cur != NULL; cur = cur->next) {
        if (cur == node) {
            prev->next = cur->next;
            cur->next = NULL;
            return TRUE;
        }
        prev = cur;
    }
    return FALSE;
}
