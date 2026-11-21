#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
} UnkStruct_022c4560;

typedef struct {
    u8 pad0[8];
    u32 unk8;
} UnkStruct_022c4680;

extern UnkStruct_022c4560 gGameState;
extern UnkStruct_022c4680 gSaveData;
extern UnkStruct_StageObj *gPlayers;

void Sound_PlaySeqArcEx(s32 a, s32 b, s32 c);
void Sound_SetPositional(s32 a, UnkStruct_StageObj *w, UnkStruct_StageObj *p);
void EnemyPawnGroup_SpawnChild(UnkStruct_StageObj *w, s32 a, s32 b, s32 c);
void EnemyPawnGroup_Action_Walk(UnkStruct_StageObj *w);
void Enemy_DisableCollidersSetAnim(UnkStruct_StageObj *w, s32 a);

void EnemyPawnGroup_State_AttackSpawnEggBishop(UnkStruct_StageObj *w) {
    if ((w->spr0.unkE >> 4) == 0x15) {
        if (!(w->unk1F4 & 4)) {
            Sound_PlaySeqArcEx(w->unk1DC, 0, 0x74);
            Sound_SetPositional(w->unk1DC, w, gPlayers);
            EnemyPawnGroup_SpawnChild(w, 0xe4, 0x30, -0x23);
        }
        w->unk1F4 |= 4;
    }
    if (!(w->unk1F4 & 1)) {
        if (w->unkC == 0)
            return;
        w->unkC--;
        if (w->unkC != 0)
            return;
        Enemy_DisableCollidersSetAnim(w, 2);
        w->unk5C |= 4;
        w->unk1F4 |= 1;
        w->unkC = 0x3a;
        if ((gSaveData.unk8 & 1) && gGameState.unk10 != 2 && gGameState.unk10 != 3)
            w->unkC += w->unkC >> 1;
        return;
    }
    w->unkC--;
    if (w->unkC == 0) {
        w->unk1F4 &= ~5;
        EnemyPawnGroup_Action_Walk(w);
    }
}
