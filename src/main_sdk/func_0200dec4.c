#include <nitro/types.h>

typedef struct UnkStruct_0200dec4 { struct UnkStruct_0200dec4 *head; u32 pad; struct UnkStruct_0200dec4 *next; } UnkStruct_0200dec4;
void func_0200dec4(UnkStruct_0200dec4 *list, UnkStruct_0200dec4 *item) {
    UnkStruct_0200dec4 *p, *prev;
    if (list == 0) return;
    p = list->head;
    prev = p;
    while (p != 0) {
        if (p == item) {
            if (p == prev) list->head = p->next; else prev->next = p->next;
            return;
        }
        prev = p;
        p = p->next;
    }
}
