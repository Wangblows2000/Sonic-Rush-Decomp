#include <game/Stage/stageobj_alt.h>

typedef struct {
    UnkStruct_EffObj base;
    u8 pad134[0x3c0 - 0x134];
    s16 unk3C0;
    s16 unk3C2;
} UnkParent_0232f1cc;

void EffVsConfusion_State_FollowPlayer(UnkStruct_EffObj *w) {
    UnkParent_0232f1cc *p = w->unk88;

    if (p == NULL)
        return;
    w->unk10 = p->base.unk10;
    w->unk14 = p->base.unk14 - 0x2000;
    if (p->unk3C0 < 0x80) {
        if (p->unk3C0 & 2)
            w->unk5C |= 0x20;
        else
            w->unk5C &= ~0x20;
    }
    if (p->unk3C0 == 0)
        w->unk54 |= 4;
}
