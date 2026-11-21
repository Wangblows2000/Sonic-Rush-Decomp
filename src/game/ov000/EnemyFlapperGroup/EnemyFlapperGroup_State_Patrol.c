#include <ov000_c/stageobj.h>

s16 ObjSpd_UpSet(s32 v, s32 a, s32 m);
void EnemyFlapperGroup_Action_PatrolTurn(UnkStruct_StageObj *w);
void EnemyFlapperGroup_UpdateHover(UnkStruct_StageObj *w);

void EnemyFlapperGroup_State_Patrol(UnkStruct_StageObj *w) {
    s16 a = 10;
    s16 m = 0xc0;
    s16 sp = 0x100;
    u16 f;

    if (w->unk1E0->unk2 == 4) {
        a = 6;
        m = 0xb0;
    }
    f = w->unk1E0->unk4;
    if (f & 3) {
        sp += ((f & 0xc) >> 2) << 7;
        a = (a * sp) >> 8;
        m = (m * sp) >> 8;
    }
    if (w->unk5C & 1)
        w->unk34 = ObjSpd_UpSet(w->unk34, -a, m);
    else
        w->unk34 = ObjSpd_UpSet(w->unk34, a, m);
    if (w->unk28 > 0 && w->unk10 > (w->unk1E8 + w->unk1E0->unk6 + w->unk1E0->unk8) << 8) {
        EnemyFlapperGroup_Action_PatrolTurn(w);
        return;
    }
    if (w->unk28 < 0 && w->unk10 < (w->unk1E8 + w->unk1E0->unk6) << 8) {
        EnemyFlapperGroup_Action_PatrolTurn(w);
        return;
    }
    if (w->unk8 != 0) {
        w->unk8--;
        if (w->unk8 == 0)
            w->coll2.unkA |= 4;
    }
    if (w->unk1E0->unk2 == 0x10)
        EnemyFlapperGroup_UpdateHover(w);
}
