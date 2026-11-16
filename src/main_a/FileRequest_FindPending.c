#include <nitro/types.h>

typedef struct UnkNode_02033d58 UnkNode_02033d58;
struct UnkNode_02033d58 { u32 unk0; UnkNode_02033d58 *next; s32 unk8; };
extern UnkNode_02033d58 *gFileRequestListHead;
extern UnkNode_02033d58 *gFileRequestListTail;

UnkNode_02033d58 *FileRequest_FindPending(void) {
    UnkNode_02033d58 *n = gFileRequestListHead->next;
    UnkNode_02033d58 *end = gFileRequestListTail;
    for (; n != end; n = n->next) {
        s32 v = n->unk8;
        if (v > -1 && (v < 5 || v > 6)) return n;
    }
    return NULL;
}
