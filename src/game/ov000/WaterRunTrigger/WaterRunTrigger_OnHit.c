#include <ov000_b/stageobj.h>

typedef struct {
    u8 unk0;
    u8 pad1[0x33];
    s16 unk34;
    u8 pad36[0x37c - 0x36];
    s16 unk37C;
} UnkPlayer_0231cffc;

typedef struct {
    u8 pad0[0x18];
    void *unk18;
} UnkColl_0231cffc;

void Player_Action_WaterRun(UnkPlayer_0231cffc *p);

void WaterRunTrigger_OnHit(UnkColl_0231cffc *a, UnkColl_0231cffc *b) {
    UnkStruct_StageObj *o = a->unk18;
    UnkPlayer_0231cffc *p = b->unk18;

    if (p == NULL)
        return;
    if (p->unk0 != 1)
        return;
    if (p->unk34 < p->unk37C || (o->unk1E0->unk4 & 1)) {
        if (p->unk34 > -p->unk37C)
            return;
        if (!(o->unk1E0->unk4 & 1))
            return;
    }
    Player_Action_WaterRun(p);
}
