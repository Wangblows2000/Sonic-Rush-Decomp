#include <nitro/types.h>

typedef struct UnkNode_02036da8 UnkNode_02036da8;
struct UnkNode_02036da8 { UnkNode_02036da8 *prev; UnkNode_02036da8 *next; };
extern s32 data_022b4578;
extern UnkNode_02036da8 *data_022b458c[];
UnkNode_02036da8 *func_02006abc(u32 a);

void func_02036da8(UnkNode_02036da8 *n) {
    if (func_02006abc(0) == n) return;
    n->prev->next = n->next;
    n->next->prev = n->prev;
    data_022b4578--;
    data_022b458c[data_022b4578] = n;
}
