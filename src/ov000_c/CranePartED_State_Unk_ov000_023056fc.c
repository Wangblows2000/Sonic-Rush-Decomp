#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x3E8 - 0x20C];
    void *unk3E8;
} UnkStruct_Carrier;

typedef struct {
    u8 pad0[0x24];
    u8 unk24[0x24];
} UnkStruct_Mdl;

extern s16 data_0206e470[];

#define SIN_IDX(i) data_0206e470[(u32)((i) << 1)]
#define COS_IDX(i) data_0206e470[(u32)(((i) << 1) + 1)]

s16 ObjSpd_UpSet(s32 v, s32 a, s32 m);
void func_0201c070(s32 a, s32 b);
void func_ov000_02305e28(void *p);
s32 ObjShift_Set(s32 v, s32 t, s32 a, s32 b, s32 c);
void func_02000dfc(void *m);
void MTX_RotY33_(void *m, s32 s, s32 c);
void func_02000f34(void *a, void *b, void *c);

void CranePartED_State_Unk_ov000_023056fc(UnkStruct_StageObj *w) {
    UnkStruct_Mdl *mdl = (UnkStruct_Mdl *)w->unk128;
    UnkStruct_Carrier *o;
    u32 mtx[9];
    s32 idx;

    if (w->unk88 != NULL) {
        w->unk8 = ((UnkStruct_StageObj *)w->unk88)->unkC;
        if (w->unk8 != ((UnkStruct_StageObj *)w->unk88)->unk8)
            w->unk54 |= 2;
        else
            w->unk54 &= ~2;
        if (((UnkStruct_StageObj *)w->unk88)->unk54 & 0x1000)
            w->unk54 |= 0x1000;
        else
            w->unk54 &= ~0x1000;
    }
    o = w->unk204;
    if (o != NULL) {
        if (w->unk14 > w->unk1EC << 8) {
            w->unk30 = ObjSpd_UpSet(w->unk30, -0x18, 0x800);
        } else {
            if (w->unk30 != 0) {
                w->unk62 = 0x80;
                func_0201c070(w->unk1DC, 0);
            }
            w->unk30 = 0;
            w->unk14 = w->unk1EC << 8;
            if (w->unk62 == 0 && (w->unk5C & 0x200)) {
                func_ov000_02305e28(w->unk88);
                w->unk5C &= ~0x200;
            }
        }
        if (o->unk3E8 != w) {
            w->unk5C |= 0x200;
            w->unk204 = NULL;
            if (w->unk88 != NULL)
                ((UnkStruct_StageObj *)w->unk88)->unk5C |= 0x200;
        }
    } else {
        func_0201c070(w->unk1DC, 0);
        w->unk30 = 0;
        w->unk20 = w->unk14;
        if (!(w->unk54 & 2))
            w->unk14 = ObjShift_Set(w->unk14, (w->unk1EC + w->unk1E0->unk7 + w->unk1E0->unk8 + w->unk1E0->unk9) << 8, 4, 0x400, 0x20);
    }
    func_02000dfc(mdl->unk24);
    idx = (s32)((u16)w->unk8 & 0xffff) >> 4;
    MTX_RotY33_(mtx, SIN_IDX(idx), COS_IDX(idx));
    func_02000f34(mdl->unk24, mtx, mdl->unk24);
}
