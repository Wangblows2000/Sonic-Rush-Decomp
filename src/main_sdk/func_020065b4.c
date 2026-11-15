#include <nitro/types.h>

typedef struct UnkStruct_020065b4 { u8 pad[0xc]; struct UnkStruct_020065b4 *prev; struct UnkStruct_020065b4 *next; } UnkStruct_020065b4;
typedef struct { u8 pad[0x7c]; UnkStruct_020065b4 *head; UnkStruct_020065b4 *tail; } UnkStruct_020065b4_Q;
void func_020065b4(UnkStruct_020065b4_Q *q, UnkStruct_020065b4 *t) {
    UnkStruct_020065b4 *tail = q->tail;
    if (tail == 0) q->head = t; else tail->next = t;
    t->prev = tail;
    t->next = 0;
    q->tail = t;
}
