#include <nitro/types.h>

typedef struct UnkStruct_0230d17c_n { u8 pad[0x38]; struct UnkStruct_0230d17c_n *next; struct UnkStruct_0230d17c_n *prev; } UnkStruct_0230d17c_n;
typedef struct { u8 pad[0xa38]; u16 cap; u16 count; u16 rd; u16 wr; u8 pad2[4]; UnkStruct_0230d17c_n **slots; } UnkStruct_0231f654;
extern UnkStruct_0231f654 *gSpecialStageMap;

void func_ov001_0230d17c(UnkStruct_0230d17c_n *n) {
    UnkStruct_0231f654 *g = gSpecialStageMap;
    n->prev->next = n->next;
    n->next->prev = n->prev;
    g->slots[g->wr] = n;
    g->wr++;
    if (g->wr >= g->cap) {
        g->wr = 0;
    }
    g->count--;
}
