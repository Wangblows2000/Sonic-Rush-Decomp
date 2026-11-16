#include <nitro/types.h>

typedef struct UnkStruct_02006590 { u8 pad[0xc]; struct UnkStruct_02006590 *prev; struct UnkStruct_02006590 *next; } UnkStruct_02006590;
typedef struct { u8 pad[0x7c]; UnkStruct_02006590 *head; UnkStruct_02006590 *tail; } UnkStruct_02006590_Q;
void OSi_DequeueItem(UnkStruct_02006590_Q *q, UnkStruct_02006590 *t) {
    UnkStruct_02006590 *next = t->next;
    UnkStruct_02006590 *prev = t->prev;
    if (next == 0) q->tail = prev; else next->prev = prev;
    if (prev == 0) q->head = next; else prev->next = next;
}
