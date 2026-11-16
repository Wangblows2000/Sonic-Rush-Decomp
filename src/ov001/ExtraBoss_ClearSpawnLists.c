#include <nitro/types.h>

typedef struct UnkStruct_02303c54_n { u8 pad[0x28]; struct UnkStruct_02303c54_n *next; } UnkStruct_02303c54_n;
typedef struct { u8 pad[0x98]; UnkStruct_02303c54_n *a; u8 pad2[4]; UnkStruct_02303c54_n *d; u8 pad3[4]; UnkStruct_02303c54_n *b; u8 pad4[4]; UnkStruct_02303c54_n *c; } UnkStruct_0231f548;
extern UnkStruct_0231f548 *gExtraBossRingWork;
void ExtraBoss_RingList_RemovePlaced(void *p);
void ExtraBoss_RingList_RemoveLoose(void *p);
void ExtraBoss_RingList_RemoveAttracted(void *p);
void ExtraBoss_RingList_RemoveCollected(void *p);
void ExtraBoss_StackPush(void *p);

void ExtraBoss_ClearSpawnLists(void) {
    UnkStruct_02303c54_n *n;
    for (n = gExtraBossRingWork->a; n != NULL; n = n->next) {
        ExtraBoss_RingList_RemovePlaced(n);
        ExtraBoss_StackPush(n);
    }
    for (n = gExtraBossRingWork->b; n != NULL; n = n->next) {
        ExtraBoss_RingList_RemoveLoose(n);
        ExtraBoss_StackPush(n);
    }
    for (n = gExtraBossRingWork->c; n != NULL; n = n->next) {
        ExtraBoss_RingList_RemoveAttracted(n);
        ExtraBoss_StackPush(n);
    }
    for (n = gExtraBossRingWork->d; n != NULL; n = n->next) {
        ExtraBoss_RingList_RemoveCollected(n);
        ExtraBoss_StackPush(n);
    }
}
