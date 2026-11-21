#include <game/Stage/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x328 - 0x20C];
    s16 unk328;
    u8 pad32A[0x358 - 0x32A];
    s16 unk358;
} UnkStruct_Other;

void Player_Action_DashPanel(UnkStruct_Other *o, s32 x, s32 y);

void DashP_OnHit(UnkStruct_Coll *a, UnkStruct_Coll *b) {
    UnkStruct_StageObj *w = a->unk18;
    UnkStruct_Other *o = b->unk18;
    s16 x = 0;
    s16 y = 0;
    u16 type;

    if (o == NULL)
        return;
    type = w->unk1E0->unk2;
    if (type != 0x5c && !(o->base.unk58 & 1)) {
        w->coll0.unkA &= ~0x100;
        return;
    }
    if (type == 0x5c && (o->unk328 < 0x1a || o->unk328 > 0x20)) {
        w->coll0.unkA &= ~0x100;
        return;
    }
    switch (type) {
    case 0x5c:
    case 0x5d:
    case 0x5e:
    case 0x5f:
    case 0x60:
    default:
        if (w->unk5C & 1)
            x = -o->unk358;
        else
            x = o->unk358;
        break;
    case 0x61:
    case 0x62:
        if (!(w->unk5C & 2))
            y = -o->unk358;
        else
            y = o->unk358;
        break;
    case 0x98:
    case 0x99:
    case 0x9a:
    case 0x9b:
        x = o->unk358;
        if (type == 0x98 || type == 0x9a)
            x = -x;
        break;
    }
    if (o != NULL)
        Player_Action_DashPanel(o, x, y);
}
