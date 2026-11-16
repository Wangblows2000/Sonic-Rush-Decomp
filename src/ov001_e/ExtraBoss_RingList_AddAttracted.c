#include <nitro/types.h>

typedef struct UnkStruct_023031e4_n {
    u8 pad0[0x24];
    struct UnkStruct_023031e4_n *prev;
    struct UnkStruct_023031e4_n *next;
} UnkStruct_023031e4_n;

typedef struct UnkStruct_023031e4 {
    u8 pad0[0xb0];
    UnkStruct_023031e4_n *head;
    UnkStruct_023031e4_n *tail;
} UnkStruct_023031e4;

extern UnkStruct_023031e4 *gExtraBossRingWork;

void ExtraBoss_RingList_AddAttracted(UnkStruct_023031e4_n *n) {
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
