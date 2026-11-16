#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x18];
    u32 unk18;
} UnkStruct_022c4560;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x23A - 0x20C];
    s16 unk23A;
} UnkStruct_022fe84c;

extern UnkStruct_022c4560 data_022c4560;

s16 ObjSpd_UpSet(s32 v, s32 a, s32 m);
void func_ov000_022fe7e8(UnkStruct_022fe84c *w);

void func_ov000_022fe84c(UnkStruct_022fe84c *w) {
    if (w->base.unkC != 0) {
        w->base.unkC--;
        if (w->base.unkC & 0x10)
            w->base.unk30 = ObjSpd_UpSet(w->base.unk30, -0x10, 0x40);
        else
            w->base.unk30 = ObjSpd_UpSet(w->base.unk30, 0x10, 0x40);
        if (w->base.unkC <= 0xc)
            w->base.unk30 = 0;
        if (w->base.unkC == 8)
            w->base.unk62 |= 0x80;
        return;
    }
    w->base.unk58 &= ~0x2000;
    w->base.unk58 |= 0x80;
    w->base.unk58 &= ~0x100;
    if (!(w->base.unk58 & 1))
        return;
    if (w->base.unk8 == 0xc) {
        w->base.unk62 |= 0x40;
        if (w->base.unk80 != NULL)
            w->unk23A = (w->base.unk10 - ((UnkStruct_StageObj *)w->base.unk80)->unk10) >> 8;
        if (w->unk23A == 0)
            w->unk23A = 1;
    }
    if (w->base.unk8 != 0) {
        w->base.unk8--;
        return;
    }
    w->base.unkC = 200;
    if (!(data_022c4560.unk18 & 0x20)) {
        w->base.spr1.unk2C |= 0x80008;
        w->base.spr1.unk2C |= 8;
    }
    func_ov000_022fe7e8(w);
}
