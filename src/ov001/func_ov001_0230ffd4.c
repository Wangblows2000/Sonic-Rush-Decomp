#include <nitro/types.h>

typedef struct UnkStruct_0230ffd4_n { u8 pad[0xdc]; struct UnkStruct_0230ffd4_n *next; struct UnkStruct_0230ffd4_n *prev; } UnkStruct_0230ffd4_n;
typedef struct { u16 cap; u16 count; u16 rd; u16 wr; u8 pad[4]; UnkStruct_0230ffd4_n **slots; u8 pad2[0x1d0]; void (*dtor)(UnkStruct_0230ffd4_n *n); } UnkStruct_0230ffd4;

void func_ov001_0230ffd4(UnkStruct_0230ffd4 *g, UnkStruct_0230ffd4_n *n) {
    if (g->dtor != NULL) {
        g->dtor(n);
    }
    n->prev->next = n->next;
    n->next->prev = n->prev;
    g->slots[g->wr] = n;
    g->wr++;
    if (g->wr >= g->cap) {
        g->wr = 0;
    }
    g->count--;
}
