#include <nitro/types.h>

typedef struct UnkNode_02033d58 UnkNode_02033d58;
struct UnkNode_02033d58 { u32 unk0; UnkNode_02033d58 *next; s32 unk8; };
extern UnkNode_02033d58 *data_0208f7ec;
extern UnkNode_02033d58 *data_0208f7e4;

UnkNode_02033d58 *func_02033d58(void) {
    UnkNode_02033d58 *n = data_0208f7ec->next;
    UnkNode_02033d58 *end = data_0208f7e4;
    for (; n != end; n = n->next) {
        s32 v = n->unk8;
        if (v > -1 && (v < 5 || v > 6)) return n;
    }
    return NULL;
}
