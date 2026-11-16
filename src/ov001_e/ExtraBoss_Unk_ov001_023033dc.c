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

extern UnkStruct_023033dc *data_ov001_0231f548;

void ExtraBoss_Unk_ov001_023033dc(UnkStruct_023033dc_n *n) {
    if (data_ov001_0231f548->tail != NULL) {
        data_ov001_0231f548->tail->next = n;
        n->prev = data_ov001_0231f548->tail;
        n->next = NULL;
        data_ov001_0231f548->tail = n;
    } else {
        data_ov001_0231f548->tail = n;
        data_ov001_0231f548->head = data_ov001_0231f548->tail;
        n->prev = n->next = NULL;
    }
}
