#include <nitro/types.h>

typedef struct UnkNode_0203d3e8 {
    struct UnkNode_0203d3e8 *next;
} UnkNode_0203d3e8;

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 e);

void func_0203d3e8(UnkNode_0203d3e8 **src, UnkNode_0203d3e8 **dst, UnkNode_0203d3e8 *n) {
    u32 e = OS_DisableInterrupts();
    UnkNode_0203d3e8 *p = *src;
    if (p == n) {
        *src = n->next;
        n->next = *dst;
        *dst = n;
    } else {
        for (; p != NULL; p = p->next) {
            if (p->next == n) {
                p->next = n->next;
                n->next = *dst;
                *dst = n;
                break;
            }
        }
    }
    OS_RestoreInterrupts(e);
}
