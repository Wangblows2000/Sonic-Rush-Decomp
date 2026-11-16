#include <ov000_b/stageobj.h>

typedef struct {
    UnkStruct_EffObj base;
    u8 pad134[0x3c0 - 0x134];
    s16 unk3C0;
    s16 unk3C2;
} UnkParent_0232f384;

void EffVsSlowdown_State_FollowPlayer(UnkStruct_EffObj *w) {
    UnkParent_0232f384 *p = w->unk88;

    if (p == NULL)
        return;
    w->unk10 = p->base.unk10;
    w->unk14 = p->base.unk14 - 0x2000;
    if (p->unk3C2 < 0x80) {
        if (p->unk3C2 & 2)
            w->unk5C |= 0x20;
        else
            w->unk5C &= ~0x20;
    }
    if (p->unk3C2 == 0)
        w->unk54 |= 4;
}
