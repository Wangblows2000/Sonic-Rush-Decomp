#include <nitro/types.h>

typedef struct UnkStruct_02007464 { u8 pad[0x14]; struct UnkStruct_02007464 *prev; struct UnkStruct_02007464 *next; } UnkStruct_02007464;
typedef struct { UnkStruct_02007464 *head; UnkStruct_02007464 *tail; } UnkStruct_0207d394;
extern UnkStruct_0207d394 data_0207d394;
void OSi_DetachVAlarm(UnkStruct_02007464 *p) {
    UnkStruct_02007464 *prev, *next;
    if (p == 0) return;
    next = p->next; prev = p->prev;
    if (next != 0) next->prev = prev; else data_0207d394.tail = prev;
    if (prev != 0) prev->next = next; else data_0207d394.head = next;
}
