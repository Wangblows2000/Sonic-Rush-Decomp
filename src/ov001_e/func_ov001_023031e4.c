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

extern UnkStruct_023031e4 *data_ov001_0231f548;

void func_ov001_023031e4(UnkStruct_023031e4_n *n) {
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
