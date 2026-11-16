#include <ov000_c/stageobj.h>

typedef struct {
    u8 unk0;
    u8 pad1;
    u8 unk2;
    u8 unk3;
    u8 pad4[0x10 - 4];
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 pad1C[0x5C - 0x1C];
    u16 unk5C;
    u8 pad5E[0x32C - 0x5E];
    u32 unk32C;
} UnkStruct_Carrier;

typedef struct {
    u8 pad0[0x24];
    u8 unk24[0x24];
} UnkStruct_Mdl;

extern s16 data_0206e470[];

void func_02000dfc(void *m);
void MTX_RotY33_(void *m, s32 s, s32 c);
void func_02000e60(void *m, s32 s, s32 c);
void func_02000f34(void *a, void *b, void *c);

void func_ov000_0230c89c(UnkStruct_StageObj *w) {
    UnkStruct_Carrier *p = w->unk204;
    UnkStruct_Mdl *mdl = (UnkStruct_Mdl *)w->unk128;
    u16 ay = 0x4000;
    u16 ax = 0;
    u32 mtx[10];
    s32 idx;

    if (p != NULL) {
        if ((w->unk1F4 & 1) && !(p->unk32C & 0x400)) {
            w->unk54 |= 4;
            return;
        }
        if (p->unk0 == 1 && (p->unk32C & 0x400)) {
            w->unk18 -= 0x200;
            w->unk1F4 |= 1;
            ay -= 0xc000;
        } else {
            w->unk10 = p->unk10;
            w->unk14 = ((UnkStruct_Carrier *)w->unk204)->unk14 + 0xc00;
            w->unk18 = ((UnkStruct_Carrier *)w->unk204)->unk18;
            p = w->unk204;
            ay -= p->unk3 << 8;
            ax = -(p->unk2 << 8);
            if (p->unk5C & 0x200) {
                w->unk2E = 0x800;
                w->unk32 = 0x40;
                w->unk58 &= ~0x2000;
                w->unk204 = NULL;
            }
        }
    }
    func_02000dfc(mdl->unk24);
    idx = (s32)(ay & 0xffff) >> 4;
    MTX_RotY33_(mtx, data_0206e470[(u32)(idx << 1)], data_0206e470[(u32)((idx << 1) + 1)]);
    func_02000f34(mdl->unk24, mtx, mdl->unk24);
    idx = (s32)(ax & 0xffff) >> 4;
    func_02000e60(mtx, data_0206e470[(u32)(idx << 1)], data_0206e470[(u32)((idx << 1) + 1)]);
    func_02000f34(mdl->unk24, mtx, mdl->unk24);
}
