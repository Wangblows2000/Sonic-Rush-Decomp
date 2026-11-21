#include <nitro/types.h>

void NNS_FndInitList(void *list, u16 offset);
u32 NewSection(void *p);

typedef struct { u32 unk0; u8 list[0xC]; } UnkStruct_0201d964;

BOOL InitHeap(UnkStruct_0201d964 *s, u32 v) {
    NNS_FndInitList(s->list, 0xC);
    s->unk0 = v;
    return NewSection(s) != 0;
}
