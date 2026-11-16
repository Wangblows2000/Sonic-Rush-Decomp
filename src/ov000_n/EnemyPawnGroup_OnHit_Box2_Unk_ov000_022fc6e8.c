#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x18];
    void *unk18;
} UnkStruct_Hit;

void func_ov000_022fcd10(UnkStruct_StageObj *w);
void Player_Action_SpringLaunch(UnkStruct_StageObj *p, s16 a, s16 b);
void Player_SetSourceMapObject(UnkStruct_StageObj *p, UnkStruct_StageObj *w);

void EnemyPawnGroup_OnHit_Box2_Unk_ov000_022fc6e8(UnkStruct_Hit *a, UnkStruct_Hit *b) {
    UnkStruct_StageObj *w = a->unk18;
    UnkStruct_StageObj *p = b->unk18;

    if (w == NULL)
        return;
    if (p == NULL)
        return;
    func_ov000_022fcd10(w);
    {s32 y = -0x780; y -= w->unk1E0->unk7 * 0x180;
    Player_Action_SpringLaunch(p, 0, y);}
    Player_SetSourceMapObject(p, w);
}
