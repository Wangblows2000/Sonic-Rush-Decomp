#include <nitro/types.h>

typedef struct UnkStruct_02303934_n {
    s32 x;
    s32 y;
    s32 ax;
    s32 ay;
    s32 vx;
    s32 vy;
    u8 pad18[8];
    s32 timer;
    u8 pad24[4];
    struct UnkStruct_02303934_n *next;
} UnkStruct_02303934_n;

typedef struct UnkStruct_02303934 {
    u8 pad0[0x98];
    UnkStruct_02303934_n *list98;
    u8 pad9C[4];
    UnkStruct_02303934_n *listA0;
    u8 padA4[4];
    UnkStruct_02303934_n *listA8;
} UnkStruct_02303934;

extern UnkStruct_02303934 *gExtraBossRingWork;

BOOL ExtraBoss_IsOutOfField(UnkStruct_02303934_n *n);
void ExtraBoss_RingList_RemovePlaced(UnkStruct_02303934_n *n);
void ExtraBoss_StackPush(UnkStruct_02303934_n *n);
void ExtraBoss_RingList_RemoveLoose(UnkStruct_02303934_n *n);
void ExtraBoss_RingList_RemoveCollected(UnkStruct_02303934_n *n);

void ExtraBoss_UpdateRings(void) {
    UnkStruct_02303934_n *n;
    UnkStruct_02303934_n *next;

    next = gExtraBossRingWork->list98;
    while (next != NULL) {
        n = next;
        next = n->next;
        n->x -= 0x4000;
        if (ExtraBoss_IsOutOfField(n)) {
            ExtraBoss_RingList_RemovePlaced(n);
            ExtraBoss_StackPush(n);
        }
    }
    next = gExtraBossRingWork->listA8;
    while (next != NULL) {
        n = next;
        next = n->next;
        n->vx += n->ax;
        n->vy += n->ay;
        n->x += n->vx;
        n->y += n->vy;
        if (ExtraBoss_IsOutOfField(n)) {
            ExtraBoss_RingList_RemoveLoose(n);
            ExtraBoss_StackPush(n);
        }
    }
    next = gExtraBossRingWork->listA0;
    while (next != NULL) {
        n = next;
        next = n->next;
        n->x -= 0x4000;
        n->timer--;
        if (n->timer < 0) {
            ExtraBoss_RingList_RemoveCollected(n);
            ExtraBoss_StackPush(n);
        }
    }
}
