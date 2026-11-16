#include <nitro/types.h>

typedef struct {
    u8 unk0;
    u8 pad1[0x80 - 1];
    void *unk80;
} UnkStruct_02319c34_Ring;

typedef struct {
    u8 pad0[0x24];
    s32 mtx[9];
} UnkStruct_02319c34_Mdl;

typedef struct {
    u8 pad0[0x62];
    u16 unk62;
} UnkStruct_02319c34_Stage;

typedef struct {
    u8 pad0[0x32c];
    u32 unk32C;
} UnkStruct_02319c34_Plr;

typedef struct {
    u8 pad0[0x20];
    u32 unk20;
} UnkStruct_02319c34_Sys;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3[5];
    u32 unk8;
    u8 padC[0x14 - 0xC];
    s32 unk14;
    s32 unk18;
    u8 pad1C[0x30 - 0x1C];
    s16 unk30;
    u8 pad32[2];
    s16 unk34;
    u8 pad36[0x54 - 0x36];
    u32 unk54;
    u32 unk58;
    u8 pad5C[0x128 - 0x5C];
    UnkStruct_02319c34_Mdl *unk128;
    u8 pad12C[0x1BC - 0x12C];
    UnkStruct_02319c34_Ring *unk1BC;
    u8 pad1C0[0x1DC - 0x1C0];
    void *unk1DC;
    u8 pad1E0[0x1F4 - 0x1E0];
    u32 unk1F4;
    u8 pad1F8[0x204 - 0x1F8];
    UnkStruct_02319c34_Ring *unk204;
    u8 pad208[0x2BC - 0x208];
    UnkStruct_02319c34_Mdl unk2BC;
    u8 pad2E0[0x364 - 0x2BC - sizeof(UnkStruct_02319c34_Mdl)];
    u32 unk364;
    u8 pad368[0x41C - 0x368];
    u16 unk41C;
    u16 unk41E;
    s32 unk420;
} UnkStruct_02319c34;

extern UnkStruct_02319c34_Stage data_ov000_0236aad8;
extern UnkStruct_02319c34_Plr *data_ov000_0236ac60;
extern UnkStruct_02319c34_Sys data_022c4560;
extern s16 data_0206e470[];

void func_ov000_0235258c(void *a, void *b);
void func_ov000_0235283c(void *a, void *b);
void func_ov000_0235264c(void *a, void *b);
void func_02000dfc(void *m);
void func_02000e60(void *m, s32 s, s32 c);
void func_02000e98(void *m, s32 s, s32 c);
void func_02000f34(void *a, void *b, void *c);
void *func_ov000_0232e2f0(void *a, s32 b, s32 c);
void StageObj_SetOAMPriority(void *a, s32 b);
void Sound_PlaySeqArcEx(void *p, s32 a, s32 b);
u8 ObjSpd_DownSet(u8 a, s32 b);
void func_ov000_02319bb4(UnkStruct_02319c34 *w);
s32 func_0201bea8(s32 a, s32 b);
void func_0204b774(void *a, s32 b, s32 c, s32 d, s32 e);

void func_ov000_02319c34(UnkStruct_02319c34 *w) {
    u8 mtx[0x24];
    UnkStruct_02319c34_Mdl *mdl = w->unk128;
    UnkStruct_02319c34_Ring *ring;
    s32 idx;
    void *eff;

    w->unk2++;
    ring = w->unk1BC;
    if (ring != NULL && ring->unk80 == w && ring->unk0 == 1) {
        w->unk58 &= ~0x2000;
        w->unk204 = w->unk1BC;
        if (data_ov000_0236aad8.unk62 != 0 && data_ov000_0236aad8.unk62 != 0xffff) {
            func_ov000_0235258c(ring, w);
        } else {
            func_ov000_0235283c(ring, w);
        }
    }
    ring = w->unk204;
    if (ring != NULL && ring->unk0 == 1) {
        if (w->unk58 & 1) {
            ring->unk80 = w;
        } else if (data_ov000_0236aad8.unk62 != 0 && data_ov000_0236aad8.unk62 != 0xffff) {
            ring->unk80 = w;
            func_ov000_0235258c(ring, w);
        } else if (!(w->unk364 & 0x80) && (ring->unk80 == NULL || ring->unk80 != w)) {
            func_ov000_0235264c(ring, w);
            w->unk204 = NULL;
        }
    } else if (data_ov000_0236aad8.unk62 != 0 && data_ov000_0236aad8.unk62 != 0xffff) {
        ring->unk80 = w;
        func_ov000_0235258c(data_ov000_0236ac60, w);
    }
    if (w->unk34 < 0x100) {
        w->unk34 = 0x100;
    }
    func_02000dfc(mdl->mtx);
    idx = (s32)((u16)w->unk8 & 0xffff) >> 4;
    func_02000e60(mtx, data_0206e470[(u32)(idx << 1)], data_0206e470[(u32)((idx << 1) + 1)]);
    func_02000f34(mdl->mtx, mtx, mdl->mtx);
    idx = (s32)((u16)(-(w->unk1 << 8)) & 0xffff) >> 4;
    func_02000e98(mtx, data_0206e470[(u32)(idx << 1)], data_0206e470[(u32)((idx << 1) + 1)]);
    func_02000f34(mdl->mtx, mtx, mdl->mtx);
    mdl = &w->unk2BC;
    func_02000dfc(mdl->mtx);
    idx = (s32)(w->unk41C & 0xffff) >> 4;
    func_02000e98(mtx, data_0206e470[(u32)(idx << 1)], data_0206e470[(u32)((idx << 1) + 1)]);
    func_02000f34(mdl->mtx, mtx, mdl->mtx);
    if (!(w->unk58 & 0x2000)) {
        w->unk41C -= w->unk34;
    }
    w->unk41E = w->unk41C;
    if (!(w->unk58 & 0x2000)) {
        w->unk420 += w->unk34;
    }
    if (data_ov000_0236aad8.unk62 != 0 && data_ov000_0236aad8.unk62 != 0xffff) {
        if (!(w->unk364 & 0x80)) {
            eff = func_ov000_0232e2f0(w, 0, 0);
            if (eff != NULL) {
                StageObj_SetOAMPriority(eff, 1);
            }
            Sound_PlaySeqArcEx(NULL, 0, 100);
            w->unk364 |= 0x80;
            w->unk30 = -0x400;
            w->unk58 |= 0x10;
        } else if (!(data_022c4560.unk20 & 7)) {
            eff = func_ov000_0232e2f0(w, 0, 0);
            if (eff != NULL) {
                StageObj_SetOAMPriority(eff, 1);
            }
            Sound_PlaySeqArcEx(NULL, 0, 100);
        }
    }
    if (w->unk364 & 0x80) {
        w->unk1 = ObjSpd_DownSet(w->unk1, 1);
        w->unk8 += 0x80;
        if (w->unk8 > 0x1880) {
            w->unk8 = 0x1880;
        }
        w->unk18 += 0x180;
        if (w->unk18 > 0xf00) {
            w->unk18 = 0xf00;
        }
    }
    if (data_ov000_0236aad8.unk62 != 0 && w->unk14 >= (data_ov000_0236aad8.unk62 << 8)) {
        func_ov000_02319bb4(w);
        return;
    }
    if (!(w->unk58 & 0x2000) && func_0201bea8(0, 0x5e) == 0) {
        func_0204b774(w->unk1DC, 0, 0x5e, 0x20, -1);
    }
    if (data_ov000_0236ac60->unk32C & 0x400) {
        w->unk1F4 |= 1;
    }
    if (w->unk1F4 & 1) {
        if (!(data_ov000_0236ac60->unk32C & 0x400)) {
            w->unk54 |= 4;
        }
    }
}
