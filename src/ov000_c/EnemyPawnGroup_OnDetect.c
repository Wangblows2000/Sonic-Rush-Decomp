#include <ov000_c/stageobj.h>

extern UnkStruct_StageObj *gPlayers;

void Sound_PlaySeqArcEx(s32 a, s32 b, s32 c);
void Sound_SetPositional(s32 a, UnkStruct_StageObj *w, UnkStruct_StageObj *p);
void NNS_SndPlayerSetTrackPitch(s32 a, s32 b, s32 c);
void EffFound_Create(UnkStruct_StageObj *w, s32 a, s32 b);
void EnemyPawnGroup_Action_Unk_ov000_022fdf28(UnkStruct_StageObj *w);
void EnemyPawnGroup_Action_Unk_ov000_022fdb9c(UnkStruct_StageObj *w);
void EnemyPawnGroup_Action_AttackSpawnEggPawnM(UnkStruct_StageObj *w);
void EnemyPawnGroup_Action_AttackSpawnEggBishop(UnkStruct_StageObj *w);
void EnemyPawnGroup_Action_Unk_ov000_022fd564(UnkStruct_StageObj *w);
void EnemyPawnGroup_Action_AttackSpawnEggSf(UnkStruct_StageObj *w);
void EnemyPawnGroup_Action_AttackSpawnGunHunter(UnkStruct_StageObj *w);
void EnemyPawnGroup_Action_Brake(UnkStruct_StageObj *w);
void EnemyPawnGroup_Action_Unk_ov000_022fcc50(UnkStruct_StageObj *w);

void EnemyPawnGroup_OnDetect(UnkStruct_Coll *a, UnkStruct_Coll *b) {
    UnkStruct_StageObj *w = a->unk18;
    UnkStruct_StageObj *o = b->unk18;

    if (w == NULL)
        return;
    if (o == NULL)
        return;
    if (w->unk54 & 2)
        return;
    if (o == NULL)
        return;
    Sound_PlaySeqArcEx(w->unk1DC, 0, 0x79);
    Sound_SetPositional(w->unk1DC, w, gPlayers);
    NNS_SndPlayerSetTrackPitch(w->unk1DC, 0xffff, 0x400);
    EffFound_Create(w, 0, -0x40);
    w->unk204 = o;
    if (w->unk10 > o->unk10)
        w->unk5C |= 1;
    else
        w->unk5C &= ~1;
    if (w->unk5 != 0)
        w->unk5C ^= 1;
    switch (w->unk1E0->unk2) {
    case 0:
    case 0x14:
        EnemyPawnGroup_Action_Unk_ov000_022fdf28(w);
        return;
    case 3:
        EnemyPawnGroup_Action_Unk_ov000_022fdb9c(w);
        return;
    case 1:
        EnemyPawnGroup_Action_AttackSpawnEggPawnM(w);
        return;
    case 7:
    case 0x15:
        EnemyPawnGroup_Action_AttackSpawnEggBishop(w);
        return;
    case 8:
        EnemyPawnGroup_Action_Unk_ov000_022fd564(w);
        return;
    case 9:
        EnemyPawnGroup_Action_AttackSpawnEggSf(w);
        return;
    case 10:
        EnemyPawnGroup_Action_Unk_ov000_022fdf28(w);
        return;
    case 0xd:
        EnemyPawnGroup_Action_AttackSpawnGunHunter(w);
        return;
    case 0xe:
        EnemyPawnGroup_Action_Brake(w);
        return;
    case 0xf:
        w->unk204 = o;
        EnemyPawnGroup_Action_Unk_ov000_022fcc50(w);
        break;
    }
}
