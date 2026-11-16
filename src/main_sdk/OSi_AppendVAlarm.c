#include <nitro/types.h>

typedef struct UnkStruct_0200749c { u8 pad[0x14]; struct UnkStruct_0200749c *prev; struct UnkStruct_0200749c *next; } UnkStruct_0200749c;
typedef struct { UnkStruct_0200749c *head; UnkStruct_0200749c *tail; } UnkStruct_0207d394;
extern UnkStruct_0207d394 data_0207d394;
void OSi_SetNextVAlarm(UnkStruct_0200749c *p);
void OSi_AppendVAlarm(UnkStruct_0200749c *p) {
    UnkStruct_0200749c *tail = data_0207d394.tail;
    p->prev = tail;
    p->next = 0;
    data_0207d394.tail = p;
    if (tail != 0) { tail->next = p; return; }
    data_0207d394.head = p;
    OSi_SetNextVAlarm(p);
}
