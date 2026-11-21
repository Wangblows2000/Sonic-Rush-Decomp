#include <game/Stage/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 unk20C[0x238 - 0x20C];
    u32 unk238;
} UnkStruct_02301738;

void Kuragen_UpdatePulse(UnkStruct_02301738 *w);
s16 ObjSpd_UpSet(s32 v, s32 a, s32 m);
void Kuragen_Action_FloatTurn(UnkStruct_02301738 *w);
void StageObj_HandleCollider(void *w, void *a);

void Kuragen_State_Float(UnkStruct_02301738 *w) {
    s16 sp = 0x100;
    s32 f;

    Kuragen_UpdatePulse(w);
    f = w->base.unk1E0->unk4 & 0xc;
    if (f)
        sp += (f >> 2) << 7;
    if (w->unk238 & 0x10) {
        w->base.unk30 = ObjSpd_UpSet(w->base.unk30, -((sp * 5) >> 8), (sp * 0x60) >> 8);
        if (w->base.unk14 < (w->base.unk1EC + w->base.unk1E0->unk7) << 8) {
            Kuragen_Action_FloatTurn(w);
            return;
        }
    } else {
        w->base.unk30 = ObjSpd_UpSet(w->base.unk30, (sp * 5) >> 8, (sp * 0x60) >> 8);
        if (w->base.unk14 > (w->base.unk1EC + w->base.unk1E0->unk7 + w->base.unk1E0->unk9) << 8) {
            Kuragen_Action_FloatTurn(w);
            return;
        }
    }
    if (!(w->unk238 & 1)) {
        if (!(w->unk238 & 0x100))
            return;
        w->unk238 &= ~0x100;
        w->unk238 |= 1;
        return;
    }
    StageObj_HandleCollider(w, w->unk20C);
}
