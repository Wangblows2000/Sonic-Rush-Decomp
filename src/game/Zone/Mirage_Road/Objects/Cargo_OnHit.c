#include <game/Stage/stageobj.h>

typedef struct {
    u8 pad0[0x18];
    void *unk18;
} UnkStruct_Hit;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x37A - 0x20C];
    s16 unk37A;
} UnkStruct_Player;

void Cargo_Action_StartMove(UnkStruct_StageObj *w);
void Player_Action_Cargo(UnkStruct_Player *p, UnkStruct_StageObj *w, s32 a, s32 b);

void Cargo_OnHit(UnkStruct_Hit *a, UnkStruct_Hit *b) {
    UnkStruct_StageObj *w = a->unk18;
    UnkStruct_Player *p = b->unk18;
    s32 v;

    if (w == NULL)
        return;
    if (p == NULL)
        return;
    if (p->base.unk0 != 1)
        return;
    v = p->base.unk28;
    if ((v < 0 ? -v : v) < p->unk37A)
        return;
    if ((w->unk1E0->unk4 & 2) || v <= 0) {
        if (!(w->unk1E0->unk4 & 2))
            return;
        if (v >= 0)
            return;
    }
    Cargo_Action_StartMove(w);
    if (w->unk1E0->unk4 & 2)
        Player_Action_Cargo(p, w, 0x18, 0);
    else
        Player_Action_Cargo(p, w, -0x18, 0);
}
