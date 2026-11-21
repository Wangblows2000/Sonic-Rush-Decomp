#include <game/Stage/stageobj_alt.h>

typedef struct {
    u8 pad0[0x28];
    s16 unk28;
    u8 pad2A[0x58 - 0x2a];
    u32 unk58;
} UnkPlayer_02322acc;

typedef struct {
    u8 pad0[0x18];
    void *unk18;
} UnkColl_02322acc;

void DecVine_OnHit(UnkColl_02322acc *a, UnkColl_02322acc *b) {
    s32 v;
    UnkPlayer_02322acc *p;
    UnkStruct_EffObj *o;

    o = a->unk18;
    p = b->unk18;
    if (o == NULL)
        return;
    if (!(p->unk58 & 1))
        return;
    v = p->unk28;
    if (v < 0)
        v = -v;
    if (v <= 0x200)
        return;
    ((u8 *)o)[7] = Rand() & 1;
}
