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

extern UnkStruct_02303334 *data_ov001_0231f548;

void ExtraBoss_Unk_ov001_02303334(UnkStruct_02303334_n *n) {
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
