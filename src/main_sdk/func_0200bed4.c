#include <nitro/types.h>

typedef struct UnkStruct_0200bed4 { struct UnkStruct_0200bed4 *prev; struct UnkStruct_0200bed4 *next; } UnkStruct_0200bed4;
void func_0200bed4(UnkStruct_0200bed4 *p, UnkStruct_0200bed4 *list) {
    UnkStruct_0200bed4 *prev = p->prev;
    UnkStruct_0200bed4 *next = p->next;
    if (prev != 0) prev->next = next;
    if (next != 0) next->prev = prev;
    while (list->next != 0) list = list->next;
    list->next = p;
    p->prev = list;
    p->next = 0;
}
