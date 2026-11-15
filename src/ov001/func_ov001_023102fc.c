#include <nitro/types.h>

typedef struct UnkStruct_023102fc_n { u8 pad[0x70]; s32 v70; u8 pad2[0x68]; struct UnkStruct_023102fc_n *next; struct UnkStruct_023102fc_n *prev; u8 pad3[4]; } UnkStruct_023102fc_n;
typedef struct { u16 cap; u16 count; u16 rd; u16 wr; u8 pad[4]; UnkStruct_023102fc_n **slots; u8 pad2[0xe4]; UnkStruct_023102fc_n list; } UnkStruct_023102fc;
void MI_CpuFill8(void *dest, u8 data, u32 size);

UnkStruct_023102fc_n *func_ov001_023102fc(UnkStruct_023102fc *g) {
    UnkStruct_023102fc_n *n;
    if (g->count >= g->cap) {
        return NULL;
    }
    g->count++;
    n = g->slots[g->rd];
    g->rd++;
    if (g->rd >= g->cap) {
        g->rd = 0;
    }
    MI_CpuFill8(n, 0, 0xe4);
    n->v70 = 99999;
    n->next = &g->list;
    n->prev = g->list.prev;
    g->list.prev->next = n;
    g->list.prev = n;
    return n;
}
