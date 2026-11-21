#include <nitro/types.h>

typedef struct UnkStruct_0200749c { u8 pad[0x14]; struct UnkStruct_0200749c *prev; struct UnkStruct_0200749c *next; } UnkStruct_0200749c;
typedef struct { UnkStruct_0200749c *head; UnkStruct_0200749c *tail; } UnkStruct_0207d394;
extern UnkStruct_0207d394 OSi_VAlarmQueue;
void OSi_SetNextVAlarm(UnkStruct_0200749c *p);
void OSi_AppendVAlarm(UnkStruct_0200749c *p) {
    UnkStruct_0200749c *tail = OSi_VAlarmQueue.tail;
    p->prev = tail;
    p->next = 0;
    OSi_VAlarmQueue.tail = p;
    if (tail != 0) { tail->next = p; return; }
    OSi_VAlarmQueue.head = p;
    OSi_SetNextVAlarm(p);
}
