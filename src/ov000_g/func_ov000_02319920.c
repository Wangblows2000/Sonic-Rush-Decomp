#include <nitro/types.h>

typedef struct {
    u8 unk0;
    u8 pad1[0x80 - 1];
    void *unk80;
} UnkStruct_02319920_Ring;

typedef struct {
    u8 pad0[0x24];
    s32 mtx[9];
} UnkStruct_02319920_Mdl;

typedef struct {
    u8 pad0[0xE];
    u16 unkE;
} UnkStruct_02319920_Stage;

typedef struct {
    u8 pad0[0x32c];
    u32 unk32C;
} UnkStruct_02319920_Plr;

typedef struct {
    u8 pad0[0x20];
    u32 unk20;
} UnkStruct_02319920_Sys;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3[5];
    u32 unk8;
    u8 padC[0x10 - 0xC];
    s32 unk10;
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
    UnkStruct_02319920_Mdl *unk128;
    u8 pad12C[0x1BC - 0x12C];
    UnkStruct_02319920_Ring *unk1BC;
    u8 pad1C0[0x1DC - 0x1C0];
    void *unk1DC;
    u8 pad1E0[0x1F4 - 0x1E0];
    u32 unk1F4;
    u8 pad1F8[0x204 - 0x1F8];
    UnkStruct_02319920_Ring *unk204;
    u8 pad208[0x2BC - 0x208];
    UnkStruct_02319920_Mdl unk2BC;
    u8 pad2E0[0x364 - 0x2BC - sizeof(UnkStruct_02319920_Mdl)];
    u32 unk364;
    u8 pad368[0x41C - 0x368];
    u16 unk41C;
    u16 unk41E;
    s32 unk420;
    u8 pad424[2];
    s16 unk426;
    u16 unk428;
} UnkStruct_02319920;

extern UnkStruct_02319920_Stage data_ov000_0236abd8;
extern UnkStruct_02319920_Plr *gPlayers;
extern u32 gObjManagerFlags;
extern s16 data_0206e470[];

void func_ov000_02319af8(UnkStruct_02319920 *w);
void func_ov000_023524d8(void *a, void *b);
void func_ov000_0235258c(void *a, void *b);
void MTX_Identity33_(void *m);
void MTX_RotX33_(void *m, s32 s, s32 c);
void MTX_RotZ33_(void *m, s32 s, s32 c);
void MTX_Concat33(void *a, void *b, void *c);

void func_ov000_02319920(UnkStruct_02319920 *w) {
    u8 mtx[0x24];
    UnkStruct_02319920_Mdl *mdl = w->unk128;
    UnkStruct_02319920_Ring *ring;
    s32 idx;

    func_ov000_02319af8(w);
    if ((w->unk10 >> 8) >= data_ov000_0236abd8.unkE - 0x180)
        w->unk34 = 0;
    w->unk426++;
    ring = w->unk1BC;
    if (ring != NULL && ring->unk80 == w && ring->unk0 == 1) {
        w->unk58 &= ~0x2000;
        w->unk204 = w->unk1BC;
        func_ov000_023524d8(ring, w);
    }
    if (w->unk204 != NULL) {
        w->unk204->unk80 = w;
        if (w->unk204->unk0 == 1 && (w->unk1BC == NULL || w->unk1BC->unk80 != w) && !(gObjManagerFlags & 0x100))
            func_ov000_0235258c(w->unk204, w);
    }
    MTX_Identity33_(mdl->mtx);
    idx = (s32)((u16)w->unk8 & 0xffff) >> 4;
    MTX_RotX33_(mtx, data_0206e470[(u32)(idx << 1)], data_0206e470[(u32)((idx << 1) + 1)]);
    MTX_Concat33(mdl->mtx, mtx, mdl->mtx);
    idx = (s32)(w->unk428 & 0xffff) >> 4;
    MTX_RotZ33_(mtx, data_0206e470[(u32)(idx << 1)], data_0206e470[(u32)((idx << 1) + 1)]);
    MTX_Concat33(mdl->mtx, mtx, mdl->mtx);
    if (gPlayers->unk32C & 0x400) {
        w->unk1F4 |= 1;
    }
    if (w->unk1F4 & 1) {
        if (!(gPlayers->unk32C & 0x400)) {
            w->unk54 |= 4;
        }
    }
}
