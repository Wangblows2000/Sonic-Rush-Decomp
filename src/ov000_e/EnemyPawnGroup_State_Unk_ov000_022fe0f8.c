#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    UnkStruct_Coll coll3;
    s16 unk238;
} UnkStruct_022fe0f8;

s16 ObjSpd_DownSet(s32 a, s32 b);
void StageObj_HandleCollider(void *w, void *a);
void ObjRect_SetBox2D(UnkStruct_Coll *c, s16 a, s16 b, s16 d, s16 e);
void func_ov000_022faf48(UnkStruct_022fe0f8 *w, s32 a);
void EnemyPawnGroup_Action_Unk_ov000_022fe7e8(UnkStruct_022fe0f8 *w);

void EnemyPawnGroup_State_Unk_ov000_022fe0f8(UnkStruct_022fe0f8 *w) {
    s16 v = 0x100;

    if (w->base.unk1E0->unk4 & 3) {
        v += ((w->base.unk1E0->unk4 & 0xc) >> 2) * 0x80;
    }
    w->base.unk34 = ObjSpd_DownSet(w->base.unk34, (v << 6) >> 8);
    if ((w->base.spr0.unkE >> 4) == 0xf) {
        w->base.unk5C ^= 1;
        w->base.unk54 |= 0x1000;
    }
    if ((w->base.spr0.unkE >> 4) == 7 && (w->base.unk54 & 0x1000)) {
        func_ov000_022faf48(w, 1);
        w->base.unk54 &= ~0x1000;
    }
    if (w->base.unk5C & 8) {
        w->unk238--;
        if (w->unk238 == 0) {
            ObjRect_SetBox2D(&w->coll3, -0x78, -0x30, -0x10, -0x10);
            EnemyPawnGroup_Action_Unk_ov000_022fe7e8(w);
            return;
        }
    }
    StageObj_HandleCollider(w, &w->coll3);
}
