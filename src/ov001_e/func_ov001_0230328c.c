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

extern UnkStruct_0230328c *data_ov001_0231f548;

void func_ov001_0230328c(UnkStruct_0230328c_n *n) {
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
