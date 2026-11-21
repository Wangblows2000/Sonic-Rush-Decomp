#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0x28];
    s16 unk28;
    s16 unk2A;
} UnkPlayer_0232112c;

typedef struct {
    UnkStruct_EffObj base;
    u8 pad134[0x190 - 0x134];
    UnkPlayer_0232112c *unk190;
} UnkWork_0232112c;

typedef struct {
    u8 pad0[0x18];
    void *unk18;
} UnkColl_0232112c;

void DecGroup41_Action_Flee(UnkWork_0232112c *w, UnkPlayer_0232112c *p);

void DecGroup41_OnHit(UnkColl_0232112c *a, UnkColl_0232112c *b) {
    UnkWork_0232112c *w;
    UnkPlayer_0232112c *p;
    s32 v;

    p = b->unk18;
    w = a->unk18;
    v = p->unk28;
    if (v < 0)
        v = -v;
    if (v <= 0x300) {
        v = p->unk2A;
        if (v < 0)
            v = -v;
        if (v <= 0x200)
            return;
    }
    DecGroup41_Action_Flee(w, p);
    w->unk190 = p;
}
