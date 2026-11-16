#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x23A - 0x20C];
    s16 unk23A;
} UnkStruct_022fdc3c;

BOOL Stage_IsGravityFlipped(s32 x, s32 y);
s16 ObjSpd_UpSet(s32 v, s32 a, s32 m);
void Enemy_DisableCollidersSetAnim(UnkStruct_022fdc3c *w, s32 a);
void EnemyPawnGroup_Action_Turn(UnkStruct_022fdc3c *w);

void EnemyPawnGroup_State_Unk_ov000_022fdc3c(UnkStruct_022fdc3c *w) {
    u16 f;
    s16 width;
    s16 sp;
    s32 base;
    s32 lim;

    base = w->base.unk1E8;
    width = 0x180;
    sp = 0x100;

    if (w->unk23A != 0 && w->base.unk80 != NULL)
        base = (s16)(w->unk23A + (((UnkStruct_StageObj *)w->base.unk80)->unk10 >> 8));
    f = w->base.unk1E0->unk4;
    if (f & 3)
        width += (f & 3) << 7;
    if (f & 3)
        sp += ((f & 0xc) >> 2) << 7;
    if (w->base.unk204 != NULL) {
        if (((UnkStruct_StageObj *)w->base.unk204)->unk10 > w->base.unk10)
            w->base.unk5C &= ~1;
        else
            w->base.unk5C |= 1;
        if (Stage_IsGravityFlipped(w->base.unk10, w->base.unk14))
            w->base.unk5C ^= 1;
    }
    if (w->base.unk5C & 1)
        w->base.unk34 = ObjSpd_UpSet(w->base.unk34, -((sp * 0x1a) >> 8), (sp << 9) >> 8);
    else
        w->base.unk34 = ObjSpd_UpSet(w->base.unk34, (sp * 0x1a) >> 8, (sp << 9) >> 8);
    if (w->base.unk28 - w->base.unk36 > 0) {
        lim = width * (w->base.unk1E0->unk6 + w->base.unk1E0->unk8) + (base << 8);
        if (w->base.unk10 > lim) {
            w->base.unk10 = lim;
            w->base.unk34 = 0;
            Enemy_DisableCollidersSetAnim(w, 3);
        } else if (!(w->base.unk5C & 4)) {
            Enemy_DisableCollidersSetAnim(w, 2);
            w->base.unk5C |= 4;
        }
    }
    if (w->base.unk28 - w->base.unk36 < 0) {
        lim = w->base.unk1E0->unk6 * width + (base << 8);
        if (w->base.unk10 < lim) {
            w->base.unk10 = lim;
            w->base.unk34 = 0;
            Enemy_DisableCollidersSetAnim(w, 3);
        } else if (!(w->base.unk5C & 4)) {
            Enemy_DisableCollidersSetAnim(w, 2);
            w->base.unk5C |= 4;
        }
    }
    if (--w->base.unkC == 0)
        EnemyPawnGroup_Action_Turn(w);
}
