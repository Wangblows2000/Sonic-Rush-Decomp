#include <nitro/types.h>

typedef struct {
    s32 x;
    s32 y;
    s32 z;
} UnkVec_0231ee28;

typedef struct {
    u8 pad0[4];
    u16 unk4;
    u8 pad6[2];
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    UnkVec_0231ee28 unk18;
    UnkVec_0231ee28 unk24;
    UnkVec_0231ee28 unk30;
    UnkVec_0231ee28 unk3C;
} UnkStruct_0236a444;

typedef struct {
    u8 pad0[0x18];
    UnkVec_0231ee28 unk18;
    UnkVec_0231ee28 unk24;
    s16 unk30;
    u8 pad32[2];
    s32 unk34;
    s32 unk38;
    u8 pad3C[0x54 - 0x3C];
    UnkVec_0231ee28 unk54;
    UnkVec_0231ee28 unk60;
    s16 unk6C;
    u8 pad6E[2];
    s32 unk70;
    s32 unk74;
} UnkStruct_0236a48c;

typedef struct {
    u8 pad0[8];
    u8 unk8[0x4C - 8];
    u8 unk4C[0xFC - 0x4C];
    u32 unkFC;
    u8 pad100[0x240 - 0x100];
    UnkVec_0231ee28 unk240;
    UnkVec_0231ee28 unk24C;
    UnkVec_0231ee28 unk258;
} UnkStruct_02084d38;

extern UnkStruct_0236a444 data_ov000_0236a444;
extern u8 data_ov000_0236a48c[];
extern UnkStruct_02084d38 NNS_G3dGlb;
extern s16 data_0206e470[];

void G3i_PerspectiveW_(s32 s, s32 c, s32 a, s32 b, s32 d, s32 e, s32 f, void *mtx);
void G3i_LookAt_(UnkVec_0231ee28 *pos, UnkVec_0231ee28 *up, UnkVec_0231ee28 *at, s32 a, void *mtx);
void VEC_Subtract(const UnkVec_0231ee28 *a, const UnkVec_0231ee28 *b, UnkVec_0231ee28 *ab);
void VEC_Add(const UnkVec_0231ee28 *a, const UnkVec_0231ee28 *b, UnkVec_0231ee28 *ab);
s32 VEC_Mag(const UnkVec_0231ee28 *v);
s32 FX_Div(s32 a, s32 b);

static inline s32 FX_MUL(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}
#define SIN(a) data_0206e470[(((a) & 0xffff) >> 4) * 2]
#define COS(a) data_0206e470[(((a) & 0xffff) >> 4) * 2 + 1]

static inline void LookAt_0231ee28(UnkVec_0231ee28 *pos, UnkVec_0231ee28 *up, UnkVec_0231ee28 *at) {
    NNS_G3dGlb.unk240 = *pos;
    NNS_G3dGlb.unk24C = *up;
    NNS_G3dGlb.unk258 = *at;
    G3i_LookAt_(pos, up, at, 0, NNS_G3dGlb.unk4C);
    NNS_G3dGlb.unkFC &= ~0xe8;
}

void func_ov000_0231ee28(void) {
    UnkStruct_0236a444 *cam = &data_ov000_0236a444;
    UnkStruct_0236a48c *w;
    UnkVec_0231ee28 d;
    UnkVec_0231ee28 pos;
    UnkVec_0231ee28 at;
    s32 mag;

    w = (UnkStruct_0236a48c *)data_ov000_0236a48c;
    G3i_PerspectiveW_(SIN(cam->unk4), COS(cam->unk4), cam->unk10, cam->unk8, cam->unkC, cam->unk14, 0, NNS_G3dGlb.unk8);
    NNS_G3dGlb.unkFC &= ~0x50;

    VEC_Subtract(&w->unk18, &w->unk24, &d);
    d.x = FX_MUL(d.x, w->unk30);
    d.y = FX_MUL(d.y, w->unk30);
    d.z = FX_MUL(d.z, w->unk30);
    if (w->unk38 > 0) {
        mag = VEC_Mag(&d);
        if (mag <= 4) {
            d.x = d.y = d.z = 0;
        } else {
            d.x = FX_Div(d.x, mag);
            d.y = FX_Div(d.y, mag);
            d.z = FX_Div(d.z, mag);
        }
        if (w->unk34 < mag)
            w->unk34 += w->unk38;
        else
            w->unk34 = mag;
        d.x = FX_MUL(d.x, w->unk34);
        d.y = FX_MUL(d.y, w->unk34);
        d.z = FX_MUL(d.z, w->unk34);
    }
    VEC_Add(&w->unk24, &d, &w->unk24);
    cam->unk18 = w->unk24;

    VEC_Subtract(&w->unk54, &w->unk60, &d);
    d.x = FX_MUL(d.x, w->unk6C);
    d.y = FX_MUL(d.y, w->unk6C);
    d.z = FX_MUL(d.z, w->unk6C);
    if (w->unk74 > 0) {
        mag = VEC_Mag(&d);
        if (mag <= 4) {
            d.x = d.y = d.z = 0;
        } else {
            d.x = FX_Div(d.x, mag);
            d.y = FX_Div(d.y, mag);
            d.z = FX_Div(d.z, mag);
        }
        if (w->unk70 < mag)
            w->unk70 += w->unk38;
        else
            w->unk70 = mag;
        d.x = FX_MUL(d.x, w->unk70);
        d.y = FX_MUL(d.y, w->unk70);
        d.z = FX_MUL(d.z, w->unk70);
    }
    VEC_Add(&w->unk60, &d, &w->unk60);
    cam->unk24 = w->unk60;

    VEC_Add(&cam->unk24, &cam->unk3C, &at);
    VEC_Add(&cam->unk18, &cam->unk3C, &pos);
    LookAt_0231ee28(&pos, &cam->unk30, &at);
}
