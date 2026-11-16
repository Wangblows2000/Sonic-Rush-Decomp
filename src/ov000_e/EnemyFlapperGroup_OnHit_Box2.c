#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad[0x18];
    void *unk18;
} UnkStruct_Hit;

void EnemyFlapperGroup_Action_Unk_ov000_02300578(UnkStruct_StageObj *w);
void EnemyFlapperGroup_Action_Unk_ov000_02300438(UnkStruct_StageObj *w);
void EnemyFlapperGroup_Action_Unk_ov000_023000dc(UnkStruct_StageObj *w);
void EnemyFlapperGroup_Action_Unk_ov000_022ffd68(UnkStruct_StageObj *w);
u16 FX_Atan2(s32 y, s32 x);
void EffFound_Create(UnkStruct_StageObj *w, s32 a, s32 b);

void EnemyFlapperGroup_OnHit_Box2(UnkStruct_Hit *a, UnkStruct_Hit *b) {
    UnkStruct_StageObj *w = a->unk18;
    UnkStruct_StageObj *o = b->unk18;
    u16 ang;

    if (w == NULL)
        return;
    if (o == NULL)
        return;
    if (o == NULL)
        return;
    switch (w->unk1E0->unk2) {
    case 0xb:
        w->coll2.unkA &= ~4;
        EnemyFlapperGroup_Action_Unk_ov000_02300578(w);
        break;
    case 0xc:
        w->coll2.unkA &= ~4;
        EnemyFlapperGroup_Action_Unk_ov000_02300438(w);
        break;
    case 0x10:
        w->coll2.unkA &= ~4;
        w->unk204 = o;
        EnemyFlapperGroup_Action_Unk_ov000_023000dc(w);
        break;
    case 0x11:
        ang = FX_Atan2((o->unk14 - w->unk14) >> 8, (o->unk10 - w->unk10) >> 8);
        if ((ang > 0x5000 && ang < 0x7000) || (ang > 0x1000 && ang < 0x3000)) {
            w->coll2.unkA &= ~4;
            EnemyFlapperGroup_Action_Unk_ov000_022ffd68(w);
            break;
        }
        return;
    case 0x12:
        w->coll2.unkA &= ~4;
        w->unk204 = o;
        EnemyFlapperGroup_Action_Unk_ov000_023000dc(w);
        break;
    }
    EffFound_Create(w, 0, -0x20);
}
