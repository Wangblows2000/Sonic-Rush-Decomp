#include <nitro/types.h>

typedef struct UnkStruct_0230d17c_n { u8 pad[0x38]; struct UnkStruct_0230d17c_n *next; struct UnkStruct_0230d17c_n *prev; } UnkStruct_0230d17c_n;
typedef struct { u8 pad[0xa38]; u16 cap; u16 count; u16 rd; u16 wr; u8 pad2[4]; UnkStruct_0230d17c_n **slots; u8 pad3[0x40]; UnkStruct_0230d17c_n list; } UnkStruct_0231f654;
extern UnkStruct_0231f654 *gSpecialStageMap;
void MI_CpuFill8(void *dest, u8 data, u32 size);

UnkStruct_0230d17c_n *SpecialStageMap_AllocSegment(void) {
    UnkStruct_0231f654 *g = gSpecialStageMap;
    UnkStruct_0230d17c_n *n;
    if (g->count >= g->cap) {
        return NULL;
    }
    g->count++;
    n = g->slots[g->rd];
    g->rd++;
    if (g->rd >= g->cap) {
        g->rd = 0;
    }
    MI_CpuFill8(n, 0, 0x40);
    n->next = &g->list;
    n->prev = g->list.prev;
    g->list.prev->next = n;
    g->list.prev = n;
    return n;
}
