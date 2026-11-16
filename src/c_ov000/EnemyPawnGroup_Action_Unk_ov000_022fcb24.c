#include <ov000_c/stageobj.h>

extern UnkStruct_StageObj *gPlayers;

void Enemy_DisableCollidersSetAnim(UnkStruct_StageObj *w, s32 a);
void Sound_PlaySeqArcEx(s32 p, s32 a, s32 b);
void Sound_SetPositional(s32 a, UnkStruct_StageObj *w, UnkStruct_StageObj *p);
void EnemyPawnGroup_State_Unk_ov000_022fcaa8(void);

void EnemyPawnGroup_Action_Unk_ov000_022fcb24(UnkStruct_StageObj *w) {
    s32 lo;
    s32 hi;
    s32 x;
    s16 sc;
    s32 ox;

    Enemy_DisableCollidersSetAnim(w, 3);
    w->unk58 |= 0x10;
    w->unk5C |= 4;
    w->unk58 &= ~1;
    w->unk68 = EnemyPawnGroup_State_Unk_ov000_022fcaa8;
    w->unk30 = -0x2c0;
    x = w->unk8;
    ox = w->unk1E8;
    sc = 0x180;
    if (w->unk1E0->unk4 & 3)
        sc += (w->unk1E0->unk4 & 3) << 7;
    lo = w->unk1E0->unk6 * sc + (ox << 8);
    hi = sc * (w->unk1E0->unk6 + w->unk1E0->unk8) + (ox << 8);
    if (x > hi)
        x = hi;
    if (x < lo)
        x = lo;
    w->unk34 = (x - w->unk10) >> 5;
    w->unk8 = 0x3c;
    w->unkC = 0x20;
    w->unk1F4 &= ~1;
    Sound_PlaySeqArcEx(w->unk1DC, 0, 0x91);
    Sound_SetPositional(w->unk1DC, w, gPlayers);
}
