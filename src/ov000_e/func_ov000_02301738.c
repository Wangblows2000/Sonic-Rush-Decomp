#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 unk20C[0x238 - 0x20C];
    u32 unk238;
} UnkStruct_02301738;

void func_ov000_023013f0(UnkStruct_02301738 *w);
s16 func_02068df8(s32 v, s32 a, s32 m);
void func_ov000_02301728(UnkStruct_02301738 *w);
void func_02069090(void *w, void *a);

void func_ov000_02301738(UnkStruct_02301738 *w) {
    s16 sp = 0x100;
    s32 f;

    func_ov000_023013f0(w);
    f = w->base.unk1E0->unk4 & 0xc;
    if (f)
        sp += (f >> 2) << 7;
    if (w->unk238 & 0x10) {
        w->base.unk30 = func_02068df8(w->base.unk30, -((sp * 5) >> 8), (sp * 0x60) >> 8);
        if (w->base.unk14 < (w->base.unk1EC + w->base.unk1E0->unk7) << 8) {
            func_ov000_02301728(w);
            return;
        }
    } else {
        w->base.unk30 = func_02068df8(w->base.unk30, (sp * 5) >> 8, (sp * 0x60) >> 8);
        if (w->base.unk14 > (w->base.unk1EC + w->base.unk1E0->unk7 + w->base.unk1E0->unk9) << 8) {
            func_ov000_02301728(w);
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
    func_02069090(w, w->unk20C);
}
