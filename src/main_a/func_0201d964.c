#include <nitro/types.h>

void NNS_FndInitList(void *list, u16 offset);
u32 func_0201d914(void *p);

typedef struct { u32 unk0; u8 list[0xC]; } UnkStruct_0201d964;

BOOL func_0201d964(UnkStruct_0201d964 *s, u32 v) {
    NNS_FndInitList(s->list, 0xC);
    s->unk0 = v;
    return func_0201d914(s) != 0;
}
