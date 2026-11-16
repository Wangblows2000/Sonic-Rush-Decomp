#include <nitro/types.h>

typedef struct UnkStruct_023033dc_n {
    u8 pad0[0x24];
    struct UnkStruct_023033dc_n *prev;
    struct UnkStruct_023033dc_n *next;
} UnkStruct_023033dc_n;

typedef struct UnkStruct_023033dc {
    u8 pad0[0x98];
    UnkStruct_023033dc_n *head;
    UnkStruct_023033dc_n *tail;
} UnkStruct_023033dc;

extern UnkStruct_023033dc *gExtraBossRingWork;

void ExtraBoss_RingList_AddPlaced(UnkStruct_023033dc_n *n) {
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
