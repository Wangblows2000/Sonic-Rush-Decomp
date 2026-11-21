#include <nitro/types.h>

typedef struct UnkStruct_0230328c_n {
    u8 pad0[0x24];
    struct UnkStruct_0230328c_n *prev;
    struct UnkStruct_0230328c_n *next;
} UnkStruct_0230328c_n;

typedef struct UnkStruct_0230328c {
    u8 pad0[0xa8];
    UnkStruct_0230328c_n *head;
    UnkStruct_0230328c_n *tail;
} UnkStruct_0230328c;

extern UnkStruct_0230328c *gExtraBossRingWork;

void ExtraBoss_RingList_AddLoose(UnkStruct_0230328c_n *n) {
    if (gExtraBossRingWork->tail != NULL) {
        gExtraBossRingWork->tail->next = n;
        n->prev = gExtraBossRingWork->tail;
        n->next = NULL;
        gExtraBossRingWork->tail = n;
    } else {
        gExtraBossRingWork->tail = n;
        gExtraBossRingWork->head = gExtraBossRingWork->tail;
        n->prev = n->next = NULL;
    }
}
