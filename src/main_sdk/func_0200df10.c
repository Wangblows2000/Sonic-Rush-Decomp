#include <nitro/types.h>

typedef struct UnkStruct_0200df10 { struct UnkStruct_0200df10 *head; u32 pad; struct UnkStruct_0200df10 *next; } UnkStruct_0200df10;
void func_0200df10(UnkStruct_0200df10 *list, UnkStruct_0200df10 *item) {
    UnkStruct_0200df10 *p;
    if (list == 0) return;
    p = list->head;
    if (p == 0) { item->next = 0; list->head = item; return; }
    while (p->next != 0) p = p->next;
    item->next = p->next;
    p->next = item;
}
