#include <game/Stage/stageobj.h>

typedef struct {
    u8 pad0[0x24];
    u8 unk24[0x24];
} UnkStruct_0230b6a0_Mdl;

typedef struct {
    u8 pad0[3];
    u8 unk3;
    u8 pad4[0x3E8 - 4];
    void *unk3E8;
} UnkStruct_0230b6a0_Tgt;

typedef struct {
    u8 unk0;
    u8 pad1[0x80 - 1];
    void *unk80;
} UnkStruct_0230b6a0_Pl;

typedef struct {
    u8 pad0[0xC8];
    u32 unkC8;
} UnkStruct_0236aad8;

extern UnkStruct_0236aad8 gCamera;
extern s16 data_0206e470[];

void Player_Action_Hand2D(UnkStruct_0230b6a0_Pl *p, UnkStruct_StageObj *w, s32 a, s32 b);
void MTX_Identity33_(void *m);
void MTX_RotY33_(void *m, s32 s, s32 c);
void MTX_Concat33(void *a, void *b, void *c);

#define SIN_IDX(i) data_0206e470[(u32)((i) << 1)]
#define COS_IDX(i) data_0206e470[(u32)(((i) << 1) + 1)]

static inline s32 FXM(s32 a, s32 b) {
    return (a * b) >> 12;
}

void Hand2d_State_Unk_ov000_0230b6a0(UnkStruct_StageObj *w) {
    u32 mtx[9];
    UnkStruct_0230b6a0_Mdl *mdl = (UnkStruct_0230b6a0_Mdl *)w->unk128;
    UnkStruct_0230b6a0_Tgt *t;
    UnkStruct_0230b6a0_Pl *p;
    u32 f;
    s32 idx;

    if (w->unk1F4 & 2) {
        w->unk58 |= 0x2000;
        w->unk2E = 0;
        w->unk30 = 0;
        w->unk28 = 0;
        w->unk2A = 0;
        w->unk2C = 0;
        w->unk1B8 = w;
        w->unkC++;
        if (w->unkC > 0x78) {
            w->unk1F4 &= ~2;
            if (w->unk1F4 & 4) {
                w->unk1F4 |= 1;
                w->unk58 &= ~0x2000;
                w->unkC = 0x8000;
            } else {
                w->unkC = 0;
            }
        }
    } else if (w->unk204 != NULL) {
        t = w->unk204;
        if (t->unk3E8 != w)
            w->unk204 = NULL;
        else
            w->unk8 = (u16)((t->unk3 & 0xff) << 8);
    } else if (w->unk1B8 != NULL && w->unk1BC != NULL && ((UnkStruct_0230b6a0_Pl *)w->unk1BC)->unk80 == w) {
        gCamera.unkC8 |= 0x40000;
        w->unk7 = 2;
        p = w->unk1BC;
        if (p->unk0 == 1) {
            w->unk204 = p;
            Player_Action_Hand2D(p, w, 0, 0);
            w->unk1F4 |= 1;
            w->unk58 &= ~0x2000;
            w->unk1B8 = NULL;
        }
    } else {
        gCamera.unkC8 &= ~0x40000;
        if (w->unk1F4 & 1) {
            w->unk8 = w->unk8 - ((w->unk18 >> 4) + 0x370);
            w->unk8 &= 0xffff;
            w->unk1B8 = NULL;
        }
    }
    f = w->unk1F4;
    if (!(f & 1))
        return;
    if (!(f & 4)) {
        w->unkC += (w->unk18 >> 8) + 0x200;
        if (w->unkC > 0x8000) {
            w->unkC = 0x8000;
            w->unk8 = 0;
            w->unk1F4 &= ~1;
        }
    } else {
        w->unkC -= (w->unk18 >> 8) + 0x200;
        if (w->unkC < 0) {
            w->unkC = 0;
            w->unk8 = 0;
            w->unk1F4 &= ~1;
        }
    }
    MTX_Identity33_(mdl->unk24);
    idx = (s32)((u16)(-w->unk8) & 0xffff) >> 4;
    MTX_RotY33_(mtx, SIN_IDX(idx), COS_IDX(idx));
    MTX_Concat33(mdl->unk24, mtx, mdl->unk24);
    idx = (s32)((u16)w->unkC & 0xffff) >> 4;
    w->unk2E = (w->unk1E8 << 8) + 0x8000 + ((COS_IDX(idx) * -0x8000) >> 12) - w->unk10;
    w->unk32 = ((SIN_IDX(idx) << 15) >> 12) - w->unk18;
    if (w->unk1F4 & 1)
        return;
    w->unkC = 0;
    w->unk18 = 0;
    w->unk1F4 ^= 4;
    w->unk1F4 |= 2;
}
