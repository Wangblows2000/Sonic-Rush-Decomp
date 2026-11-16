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

extern UnkStruct_02303934 *data_ov001_0231f548;

BOOL ExtraBoss_Unk_ov001_02303554(UnkStruct_02303934_n *n);
void ExtraBoss_Unk_ov001_02303394(UnkStruct_02303934_n *n);
void ExtraBoss_StackPush(UnkStruct_02303934_n *n);
void ExtraBoss_Unk_ov001_02303244(UnkStruct_02303934_n *n);
void ExtraBoss_Unk_ov001_023032ec(UnkStruct_02303934_n *n);

void ExtraBoss_Unk_ov001_02303934(void) {
    UnkStruct_02303934_n *n;
    UnkStruct_02303934_n *next;

    next = data_ov001_0231f548->list98;
    while (next != NULL) {
        n = next;
        next = n->next;
        n->x -= 0x4000;
        if (ExtraBoss_Unk_ov001_02303554(n)) {
            ExtraBoss_Unk_ov001_02303394(n);
            ExtraBoss_StackPush(n);
        }
    }
    next = data_ov001_0231f548->listA8;
    while (next != NULL) {
        n = next;
        next = n->next;
        n->vx += n->ax;
        n->vy += n->ay;
        n->x += n->vx;
        n->y += n->vy;
        if (ExtraBoss_Unk_ov001_02303554(n)) {
            ExtraBoss_Unk_ov001_02303244(n);
            ExtraBoss_StackPush(n);
        }
    }
    next = data_ov001_0231f548->listA0;
    while (next != NULL) {
        n = next;
        next = n->next;
        n->x -= 0x4000;
        n->timer--;
        if (n->timer < 0) {
            ExtraBoss_Unk_ov001_023032ec(n);
            ExtraBoss_StackPush(n);
        }
    }
}
