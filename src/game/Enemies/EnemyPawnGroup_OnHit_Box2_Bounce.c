#include <game/Stage/stageobj.h>

typedef struct {
    u8 pad0[0x18];
    void *unk18;
} UnkStruct_Hit;

void EnemyPawnGroup_Action_Bounced(UnkStruct_StageObj *w);
void Player_Action_SpringLaunch(UnkStruct_StageObj *p, s16 a, s16 b);
void Player_SetSourceMapObject(UnkStruct_StageObj *p, UnkStruct_StageObj *w);

void EnemyPawnGroup_OnHit_Box2_Bounce(UnkStruct_Hit *a, UnkStruct_Hit *b) {
    UnkStruct_StageObj *w = a->unk18;
    UnkStruct_StageObj *p = b->unk18;

    if (w == NULL)
        return;
    if (p == NULL)
        return;
    EnemyPawnGroup_Action_Bounced(w);
    {s32 y = -0x780; y -= w->unk1E0->unk7 * 0x180;
    Player_Action_SpringLaunch(p, 0, y);}
    Player_SetSourceMapObject(p, w);
}
