#include <nitro/types.h>

typedef struct UnkNode_02036da8 UnkNode_02036da8;
struct UnkNode_02036da8 { UnkNode_02036da8 *prev; UnkNode_02036da8 *next; };
extern s32 gTaskPoolUsed;
extern UnkNode_02036da8 *gTaskFreeList[];
UnkNode_02036da8 *func_02006abc(u32 a);

void Task_FreeNode(UnkNode_02036da8 *n) {
    if (func_02006abc(0) == n) return;
    n->prev->next = n->next;
    n->next->prev = n->prev;
    gTaskPoolUsed--;
    gTaskFreeList[gTaskPoolUsed] = n;
}
