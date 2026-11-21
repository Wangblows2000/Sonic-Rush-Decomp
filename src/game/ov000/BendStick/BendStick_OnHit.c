#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad[0x18];
    void *unk18;
} UnkStruct_Hit;

void ObjRect_FuncNoHit(UnkStruct_Hit *a, UnkStruct_Hit *b);
void StageObj_SetAnimation(void *w, s32 a);
void Player_Action_BendStick(UnkStruct_StageObj *p, s16 a, s32 b);
void Player_SetSourceMapObject(UnkStruct_StageObj *p, UnkStruct_StageObj *w);

static inline s32 Abs(s32 x) { return x < 0 ? -x : x; }

void BendStick_OnHit(UnkStruct_Hit *a, UnkStruct_Hit *b) {
    UnkStruct_StageObj *w;
    s32 t;
    s32 d;
    UnkStruct_StageObj *p;
    s32 v;

    w = a->unk18;
    p = w->unk1BC;

    if (p != NULL) {
    if (p->unk0 == 1 && p == b->unk18 && w->unk1D6 == 0) {
        {
            s32 dx = p->unk10 - w->unk10;
            if (dx < 0)
                dx = -dx;
            d = dx >> 8;
        }
        if (d < 0x10) {
            t = 10;
            StageObj_SetAnimation(w, 3);
            v = -0x500;
            w->unk8 = 0;
        } else if (d < 0x20) {
            t = 15;
            StageObj_SetAnimation(w, 2);
            v = -0x580;
            w->unk8 = 1;
        } else {
            t = 20;
            StageObj_SetAnimation(w, 1);
            v = -0x600;
            w->unk8 = 2;
        }
        w->unkC = t;
        Player_Action_BendStick(p, v - d * 0x18, t);
        Player_SetSourceMapObject(p, w);
    } else {
        ObjRect_FuncNoHit(a, b);
    }
    } else {
        ObjRect_FuncNoHit(a, b);
    }
}
