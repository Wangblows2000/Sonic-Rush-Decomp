#include <nitro/types.h>

typedef struct UnkStruct_02303334_n {
    u8 pad0[0x24];
    struct UnkStruct_02303334_n *prev;
    struct UnkStruct_02303334_n *next;
} UnkStruct_02303334_n;

typedef struct UnkStruct_02303334 {
    u8 pad0[0xa0];
    UnkStruct_02303334_n *head;
    UnkStruct_02303334_n *tail;
} UnkStruct_02303334;

extern UnkStruct_02303334 *gExtraBossRingWork;

void ExtraBoss_RingList_AddCollected(UnkStruct_02303334_n *n) {
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
