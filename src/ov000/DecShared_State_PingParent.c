#include <nitro/types.h>

typedef struct UnkStruct_02322f68_2 {
    u8 pad[0xc];
    s32 unkC;
} UnkStruct_02322f68_2;

typedef struct UnkStruct_02322f68 {
    u8 pad[0x88];
    UnkStruct_02322f68_2 *unk88;
} UnkStruct_02322f68;

void DecShared_State_PingParent(UnkStruct_02322f68 *s) {
    UnkStruct_02322f68_2 *p = s->unk88;
    if (p != NULL)
        p->unkC++;
}
