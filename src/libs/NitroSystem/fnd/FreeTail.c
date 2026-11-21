#include <nitro/types.h>

typedef struct UnkNode_020210d0 UnkNode_020210d0;
struct UnkNode_020210d0 { u32 unk0; u32 unk4; u32 unk8; UnkNode_020210d0 *next; };
typedef struct { u32 unk0; u32 unk4; UnkNode_020210d0 *unk8; } UnkSub_020210d0;
typedef struct { u8 unk0[0x1C]; u32 unk1C; u8 unk20[4]; UnkSub_020210d0 unk24; } UnkStruct_020210d0;

void FreeTail(UnkStruct_020210d0 *s) {
    UnkNode_020210d0 *n;
    UnkSub_020210d0 *sub = &s->unk24;
    for (n = sub->unk8; n != NULL; n = n->next) {
        n->unk8 = s->unk1C;
    }
    sub->unk4 = s->unk1C;
}
