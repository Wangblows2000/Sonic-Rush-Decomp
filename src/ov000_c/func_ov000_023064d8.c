#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x14];
    s32 unk14;
    u8 pad18[0x3E8 - 0x18];
    void *unk3E8;
} UnkStruct_023064d8_Tgt;

typedef struct {
    u8 pad0[0x24];
    u8 unk24[0x24];
} UnkStruct_023064d8_Mdl;

typedef struct {
    UnkStruct_StageObj base;
    UnkStruct_Blk blk[10];
} UnkStruct_023064d8;

typedef struct {
    u8 pad0[0x20];
    u32 unk20;
} UnkStruct_022c4560;

extern UnkStruct_022c4560 data_022c4560;
extern s16 data_0206e470[];

s32 ObjSpd_AccelToward(void *w, s32 a, s32 b, s32 c, s32 d);
s16 ObjSpd_DownSet(s32 a, s32 b);
void ScreenShake_Start(s32 a);
void Sound_PlaySeqArcEx(s32 a, s32 b, s32 c);
void func_02000dfc(void *m);
void func_02000e60(void *m, s32 s, s32 c);
void func_02000f34(void *a, void *b, void *c);
void func_0201bd80(s32 a, s32 b);
void func_0201c070(s32 a, s32 b);

void func_ov000_023064d8(UnkStruct_023064d8 *w) {
    u32 mtx[9];
    UnkStruct_023064d8_Mdl *mdl;
    UnkStruct_023064d8_Tgt *tgt;
    s32 v;
    s32 a;
    s32 idx;

    tgt = w->base.unk204;
    mdl = (UnkStruct_023064d8_Mdl *)w->base.unk128;
    if (tgt != NULL) {
        if ((w->base.unk1F4 & 2) && !(w->base.unk1F4 & 1)) {
            w->base.unkC = ObjSpd_AccelToward(w, w->base.unkC, 0x20, 0x20, 0x400);
            w->base.unk2E = -w->base.unk34;
        } else {
            w->base.unk34 = ObjSpd_DownSet(w->base.unk34, 10);
            v = w->base.unk34;
            if (v < -0x800)
                v = -0x800;
            else if (v > 0x800)
                v = 0x800;
            w->base.unk2E = v;
            w->base.unkC = (w->base.unk10 - (w->base.unk1E8 << 8)) & 0xffff;
            if (w->base.unk2E == 0 && !(w->base.unk1F4 & 1)) {
                w->base.unk1F4 |= 2;
                if (w->base.unkC > 0x7fff)
                    w->base.unkC -= 0x10000;
            }
        }
        if (tgt->unk3E8 != w) {
            if (((UnkStruct_023064d8_Tgt *)w->base.unk204)->unk14 > w->base.unk14 + 0x6400)
                w->base.unk1B8 = w;
            if (((UnkStruct_023064d8_Tgt *)w->base.unk204)->unk14 < w->base.unk14 - 0x7400)
                w->base.unk1B8 = w;
            w->base.unk2E = 0;
            w->base.unk34 = w->base.unk2E;
            w->base.unk5C |= 0x200;
            w->base.unk204 = NULL;
            w->base.unk8 = 0x10;
            if (w->base.unkC > 0x7fff)
                w->base.unkC -= 0x10000;
        }
    } else if (w->base.unkC == 0 || (w->base.unk1F4 & 1)) {
        if (w->base.unk5C & 2)
            w->base.unk1B8 = w;
        w->base.unk2E = 0;
        w->base.unk34 = w->base.unk2E;
        if (w->base.unk8 == 0) {
            w->base.unk1B8 = w;
            if (!(w->base.unk54 & 0x2000))
                w->base.unk54 &= ~2;
        } else {
            w->base.unk8--;
        }
    } else {
        w->base.unkC = ObjSpd_AccelToward(w, w->base.unkC, 0x20, 0x20, 0x400);
        w->base.unk2E = -w->base.unk34;
    }

    if (w->base.unk1E0->unk6 >= 0) {
        if (w->base.unk10 + w->base.unk2E < (w->base.unk1E8 << 8)) {
            w->base.unk10 = w->base.unk1E8 << 8;
            w->base.unk2E = 0;
            w->base.unk34 = w->base.unk2E;
        }
        if (w->base.unk10 + w->base.unk2E > (w->base.unk1E8 + (w->base.unk1E0->unk6 << 6)) << 8) {
            if (!(w->base.unk1F4 & 1)) {
                w->base.unk1F4 |= 1;
                w->base.unk62 = 0x44;
                ScreenShake_Start(2);
                Sound_PlaySeqArcEx(0, 0, 0x6d);
            }
            w->base.unk10 = (w->base.unk1E8 + (w->base.unk1E0->unk6 << 6)) << 8;
            w->base.unk34 = 0;
            w->base.unk54 |= 0x2000;
            w->base.unk58 |= 0x2000;
        }
    } else {
        if (w->base.unk10 + w->base.unk2E > (w->base.unk1E8 << 8)) {
            w->base.unk10 = w->base.unk1E8 << 8;
            w->base.unk2E = 0;
            w->base.unk34 = w->base.unk2E;
        }
        if (w->base.unk10 + w->base.unk2E < (w->base.unk1E8 + (w->base.unk1E0->unk6 << 6)) << 8) {
            if (!(w->base.unk1F4 & 1)) {
                w->base.unk1F4 |= 1;
                w->base.unk62 = 0x44;
                ScreenShake_Start(2);
                Sound_PlaySeqArcEx(0, 0, 0x6d);
            }
            w->base.unk10 = (w->base.unk1E8 + (w->base.unk1E0->unk6 << 6)) << 8;
            w->base.unk34 = 0;
            w->base.unk54 |= 0x2000;
            w->base.unk58 |= 0x2000;
        }
    }

    if (w->base.unk34 != 0) {
        if (!(data_022c4560.unk20 & 8)) {
            w->blk[0].unkA8 &= ~0x80;
            w->blk[4].unkA8 = w->blk[3].unkA8 = w->blk[2].unkA8 = w->blk[1].unkA8 = w->blk[0].unkA8;
            w->blk[5].unkA8 |= 0x80;
            w->blk[9].unkA8 = w->blk[8].unkA8 = w->blk[7].unkA8 = w->blk[6].unkA8 = w->blk[5].unkA8;
        } else {
            w->blk[0].unkA8 |= 0x80;
            w->blk[4].unkA8 = w->blk[3].unkA8 = w->blk[2].unkA8 = w->blk[1].unkA8 = w->blk[0].unkA8;
            w->blk[5].unkA8 &= ~0x80;
            w->blk[9].unkA8 = w->blk[8].unkA8 = w->blk[7].unkA8 = w->blk[6].unkA8 = w->blk[5].unkA8;
        }
    } else {
        w->blk[0].unkA8 &= ~0x80;
        w->blk[4].unkA8 = w->blk[3].unkA8 = w->blk[2].unkA8 = w->blk[1].unkA8 = w->blk[0].unkA8;
        w->blk[5].unkA8 |= 0x80;
        w->blk[9].unkA8 = w->blk[8].unkA8 = w->blk[7].unkA8 = w->blk[6].unkA8 = w->blk[5].unkA8;
        if (w->base.unk62 == 0) {
            w->base.unk2E = 0;
            w->base.unk28 = 0;
        }
    }

    func_02000dfc(mdl->unk24);
    idx = (s32)((u16)w->base.unkC & 0xffff) >> 4;
    func_02000e60(mtx, data_0206e470[(u32)(idx << 1)], data_0206e470[(u32)((idx << 1) + 1)]);
    func_02000f34(mdl->unk24, mtx, mdl->unk24);

    v = w->base.unkC & 0xffff;
    if (v < 0xc000 && v < 0x4000)
        w->base.unk5C &= ~2;
    else
        w->base.unk5C |= 2;

    if (w->base.unk34 != 0)
        w->base.spr1.unk2C |= 0x80000;
    else if ((w->base.unk54 & 0x2000) || w->base.unk10 == w->base.unk1E8 << 8)
        w->base.spr1.unk2C &= ~0x80000;

    v = w->base.unk2E;
    a = v < 0 ? -v : v;
    if (a < 0x400) {
        a = v >> 2;
        if (a < 0)
            a = -a;
        if (a < 1)
            a = 1;
        else if (a > 0x100)
            a = 0x100;
        func_0201bd80(w->base.unk1DC, a);
    } else {
        func_0201bd80(w->base.unk1DC, 0x100);
    }
    if (w->base.unk2E != 0)
        return;
    func_0201c070(w->base.unk1DC, 0);
}
