#include <game/Stage/stageobj.h>

s16 ObjSpd_DownSet(s32 a, s32 b);
void EnemyFlapperGroup_Action_Patrol(UnkStruct_StageObj *w);
void EnemyFlapperGroup_UpdateHover(UnkStruct_StageObj *w);

void EnemyFlapperGroup_State_PatrolTurn(UnkStruct_StageObj *w) {
    s16 a = 0x40;
    s16 sp = 0x100;
    u16 f;

    if (w->unk1E0->unk2 == 4)
        a = 4;
    f = w->unk1E0->unk4;
    if (f & 3) {
        sp += ((f & 0xc) >> 2) << 7;
        a = (a * sp) >> 8;
    }
    w->unk34 = ObjSpd_DownSet(w->unk34, a);
    if (w->unk5C & 8) {
        w->unk5C ^= 1;
        EnemyFlapperGroup_Action_Patrol(w);
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
