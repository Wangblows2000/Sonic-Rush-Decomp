#include <ov000_c/stageobj.h>

typedef struct UnkStruct_022fb3d8_Pl {
    u8 unk0;
    u8 pad1[0x62 - 1];
    u16 unk62;
    u8 pad64[0x88 - 0x64];
    void *unk88;
    u8 pad8C[0x32C - 0x8C];
    u32 unk32C;
} UnkStruct_022fb3d8_Pl;

typedef struct {
    u8 pad0[0x18];
    void *unk18;
} UnkStruct_022fb3d8_Coll;

void ObjRect_FuncNoHit(UnkStruct_022fb3d8_Coll *a, UnkStruct_022fb3d8_Coll *b);
void func_ov000_02343000(UnkStruct_022fb3d8_Pl *pl, s32 a);
void MapObject_Action_Unk_ov000_022fafc8(UnkStruct_StageObj *w, UnkStruct_022fb3d8_Pl *pl);
void MapObject_Unk_ov000_022fa9e8(UnkStruct_StageObj *w, UnkStruct_022fb3d8_Pl *pl, s32 a);
void MapObject_Unk_ov000_022fb898(UnkStruct_StageObj *w);
void MapObject_Unk_ov000_022fb738(UnkStruct_StageObj *w);
s32 ObjRect_HitCenterX(UnkStruct_022fb3d8_Coll *a, UnkStruct_022fb3d8_Coll *b);
s32 ObjRect_HitCenterY(UnkStruct_022fb3d8_Coll *a, UnkStruct_022fb3d8_Coll *b);
void EffVs_Create(s32 x, s32 y);
void Sound_PlaySeqArcEx(s32 a, s32 b, s32 c);
void EffVitality_Create(UnkStruct_StageObj *w, s32 a, s32 b, s32 c);
void Player_Action_AttackRecoil(UnkStruct_022fb3d8_Pl *pl);
void Player_Action_AttackBounce(UnkStruct_022fb3d8_Pl *pl);

static inline s32 RND(u16 r) {
    return 0xf00 - (r & 0x1fff);
}

void MapObject_OnHit_Default(UnkStruct_022fb3d8_Coll *a, UnkStruct_022fb3d8_Coll *b) {
    UnkStruct_StageObj *w = a->unk18;
    UnkStruct_022fb3d8_Pl *pl = b->unk18;
    u16 r1;
    u16 r2;
    UnkStruct_022fb3d8_Pl *p2;

    if (w == NULL)
        return;
    if (w->unk1E5 == 0) {
        if ((u16)(w->unk1E0->unk2 + 0xff4f) <= 1) {
            if (pl->unk0 != 1) {
                w->unk54 |= 2;
                w->coll0.unkA |= 0x1000;
                w->coll1.unkA |= 0x1000;
                w->coll2.unkA |= 0x1000;
            } else {
                ObjRect_FuncNoHit(a, b);
                return;
            }
        } else {
            if (!(w->unk58 & 0x1000))
                w->unk1F4 |= 0x10;
            w->unk54 |= 2;
            w->coll0.unkA |= 0x1000;
            w->coll1.unkA |= 0x1000;
            w->coll2.unkA |= 0x1000;
        }
        if (pl != NULL) {
            if (pl->unk0 == 1) {
                func_ov000_02343000(pl, 400);
            } else if (pl->unk88 != NULL) {
                pl = pl->unk88;
                if (pl->unk0 == 1) {
                    func_ov000_02343000(pl, 400);
                    MapObject_Action_Unk_ov000_022fafc8(w, pl);
                    if (pl->unk32C & 0x80) {
                        pl->unk62 = 0x44;
                        w->unk62 = 0x44;
                    }
                    MapObject_Unk_ov000_022fa9e8(w, pl, 2);
                    return;
                }
            }
        }
        MapObject_Unk_ov000_022fb898(w);
        MapObject_Unk_ov000_022fb738(w);
        w->unk54 |= 8;
        MapObject_Unk_ov000_022fa9e8(w, pl, 2);
    } else {
        EffVs_Create(ObjRect_HitCenterX(a, b), ObjRect_HitCenterY(a, b));
        r1 = Rand();
        r2 = Rand();
        EffVs_Create(ObjRect_HitCenterX(a, b) + RND(r2), ObjRect_HitCenterY(a, b) + RND(r1));
        r1 = Rand();
        r2 = Rand();
        EffVs_Create(ObjRect_HitCenterX(a, b) + RND(r2), ObjRect_HitCenterY(a, b) + RND(r1));
        Sound_PlaySeqArcEx(0, 0, 0x7a);
        EffVitality_Create(w, 0, -0x50, w->unk1E5);
        w->unk1E5--;
        w->unk1E0->unkB++;
        w->unk1F0 = 0x3c;
        w->coll1.unk20 &= ~7;
        MapObject_Unk_ov000_022fa9e8(w, pl, 1);
        if (pl != NULL && pl->unk0 == 1) {
            Player_Action_AttackRecoil(pl);
            return;
        }
    }
    if (pl == NULL)
        return;
    if (pl->unk0 != 1)
        return;
    Player_Action_AttackBounce(pl);
}
