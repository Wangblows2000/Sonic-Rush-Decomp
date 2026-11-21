#include <nitro/types.h>

typedef struct UnkStruct_02006568 { u8 pad[0xc]; struct UnkStruct_02006568 *prev; struct UnkStruct_02006568 *next; } UnkStruct_02006568;
typedef struct { u8 pad[0x7c]; UnkStruct_02006568 *head; UnkStruct_02006568 *tail; } UnkStruct_02006568_Q;
UnkStruct_02006568 *OSi_DequeueHead(UnkStruct_02006568_Q *q) {
    UnkStruct_02006568 *t = q->head;
    UnkStruct_02006568 *next = t->next;
    if (next == 0) q->tail = 0; else next->prev = 0;
    q->head = next;
    return t;
}
