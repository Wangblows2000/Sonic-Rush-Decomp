#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x23A - 0x20C];
    s16 unk23A;
} UnkStruct_022fd990;

s16 ObjSpd_UpSet(s32 v, s32 a, s32 m);
void Enemy_DisableCollidersSetAnim(UnkStruct_022fd990 *w, s32 a);
void EnemyPawnGroup_Action_Turn(UnkStruct_022fd990 *w);

void EnemyPawnGroup_State_Charge(UnkStruct_022fd990 *w) {
    u16 f;
    s16 width;
    s16 sp;
    s32 base;

    base = w->base.unk1E8;
    width = 0x180;
    sp = 0x100;
    if (w->unk23A != 0 && w->base.unk80 != NULL)
        base = (s16)(w->unk23A + (((UnkStruct_StageObj *)w->base.unk80)->unk10 >> 8));
    if (w->base.spr0.unk8 == 2) {
        if (w->base.unkC != 0 && --w->base.unkC == 0) {
            if (w->base.unk1F4 & 8) {
                w->base.unk5C &= ~0x10;
                EnemyPawnGroup_Action_Turn(w);
                return;
            }
            w->base.unk5C &= ~0x10;
        }
        if (!(w->base.unk5C & 8))
            return;
        w->base.unkC = 0x30;
        w->base.unk1F4 |= 8;
        Enemy_DisableCollidersSetAnim(w, 3);
        w->base.unk5C |= 4;
        return;
    }
    f = w->base.unk1E0->unk4;
    if (f & 3)
        width += (f & 3) << 7;
    if (f & 3)
        sp += ((f & 0xc) >> 2) << 7;
    if (w->base.unk5C & 1)
        w->base.unk34 = ObjSpd_UpSet(w->base.unk34, -((sp * 0x140) >> 8), (sp << 10) >> 8);
    else
        w->base.unk34 = ObjSpd_UpSet(w->base.unk34, (sp * 0x140) >> 8, (sp << 10) >> 8);
    if (w->base.unk28 - w->base.unk36 > 0 && w->base.unk10 > width * (w->base.unk1E0->unk6 + w->base.unk1E0->unk8) + (base << 8))
        w->base.unk34 = 0;
    if (w->base.unk28 - w->base.unk36 < 0 && w->base.unk10 < w->base.unk1E0->unk6 * width + (base << 8))
        w->base.unk34 = 0;
    if (w->base.unk34 == 0 || (w->base.unk58 & 4)) {
        Enemy_DisableCollidersSetAnim(w, 2);
        w->base.unk5C |= 0x10;
    }
}
