#include <nitro/types.h>

typedef struct UnkStruct_0230063c_m33 {
    s32 m[9];
} UnkStruct_0230063c_m33;

typedef struct UnkStruct_0230063c_m44 {
    s32 m[16];
} UnkStruct_0230063c_m44;

typedef struct UnkStruct_0230063c_a {
    u8 pad0[0x24];
    UnkStruct_0230063c_m33 mtx;
    s32 unk48;
    s32 unk4C;
    s32 unk50;
} UnkStruct_0230063c_a;

typedef struct UnkStruct_0230063c {
    u8 pad0[0x14];
    u32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    u8 pad24[0xc];
    s32 unk30;
    s32 unk34;
    s32 unk38;
    u16 unk3C;
    u16 unk3E;
    u16 unk40;
} UnkStruct_0230063c;

typedef struct UnkStruct_0230063c_c {
    u16 unk0;
    u8 pad2[2];
    s32 unk4;
    s32 unk8;
    s32 unkC;
    u8 pad10[4];
    s32 unk14;
    s32 unk18;
    s32 unk1C;
} UnkStruct_0230063c_c;

typedef struct UnkStruct_0230063c_d38 {
    u8 pad[0xfc];
    u32 unkFC;
} UnkStruct_0230063c_d38;

extern s32 data_ov001_0231f524;
extern s32 data_ov001_0231f520;
extern s16 data_0206e470[];
extern UnkStruct_0230063c_m44 data_02084d40;
extern UnkStruct_0230063c_d38 NNS_G3dGlb;

void MTX_RotX33_(void *m, s32 s, s32 c);
void MTX_RotY33_(void *m, s32 s, s32 c);
void MTX_RotZ33_(void *m, s32 s, s32 c);
void MTX_Concat33(void *a, void *b, void *c);
UnkStruct_0230063c_c *func_ov001_022f71f4(void);
s32 func_0200225c(s32 n, s32 d);
void G3i_FrustumW_(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, void *m);
void Model_Animate(void *p);
void Model_Draw(void *p);

#define SIN(a) data_0206e470[(((a) & 0xffff) >> 4) * 2]
#define COS(a) data_0206e470[(((a) & 0xffff) >> 4) * 2 + 1]

static inline s32 FX_Mul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

void func_ov001_0230063c(UnkStruct_0230063c *w, UnkStruct_0230063c_a *a) {
    UnkStruct_0230063c_m33 m;
    UnkStruct_0230063c_m44 saved;
    u32 fl = w->unk14;
    u16 ang;
    BOOL cam;
    UnkStruct_0230063c_c *c;
    s32 t;
    s32 x;
    s32 y;
    s32 d;
    s32 dx;
    s32 dy;

    ang = 0;
    if (fl & 0x20) {
        if (fl & 0x40)
            ang = 0xc000;
        else
            ang = 0x4000;
    }
    a->unk48 = data_ov001_0231f524 + (w->unk18 + w->unk30);
    a->unk4C = -(data_ov001_0231f520 + (w->unk1C + w->unk34));
    a->unk50 = w->unk20 + w->unk38;
    MTX_RotY33_(&a->mtx, SIN(ang), COS(ang));
    MTX_RotX33_(&m, SIN(w->unk3C), COS(w->unk3C));
    MTX_Concat33(&a->mtx, &m, &a->mtx);
    MTX_RotY33_(&m, SIN(w->unk3E), COS(w->unk3E));
    MTX_Concat33(&a->mtx, &m, &a->mtx);
    MTX_RotZ33_(&m, SIN(w->unk40), COS(w->unk40));
    MTX_Concat33(&a->mtx, &m, &a->mtx);
    cam = fl & 1;
    if (cam) {
        c = func_ov001_022f71f4();
        x = FX_Mul(func_0200225c(SIN(c->unk0), COS(c->unk0)), c->unk4);
        y = FX_Mul(x, c->unkC);
        d = func_0200225c(c->unk4, c->unk1C);
        dy = FX_Mul(d, c->unk18 - a->unk4C);
        dx = FX_Mul(d, c->unk14 - a->unk48);
        a->unk48 = c->unk14;
        a->unk4C = c->unk18;
        saved = data_02084d40;
        G3i_FrustumW_(x + dy, dy - x, dx - y, y + dx, c->unk4, c->unk8, 0x1000, 0, &data_02084d40);
        NNS_G3dGlb.unkFC &= ~0x50;
    }
    if (!(fl & 0x10000))
        Model_Animate(a);
    Model_Draw(a);
    if (cam)
        data_02084d40 = saved;
}
