#include <nitro/types.h>

typedef struct UnkStruct_02304a24 {
    u32 flags;
    u8 pad4[0x588 - 0x4];
    s32 unk588;
    u8 pad58c[0x5ac - 0x58c];
    u16 hdma[2][0xc0];
    s16 wave[0x140];
    u16 unkB2C;
    u8 padB2E[2];
    s32 unkB30;
    s32 unkB34;
    s32 unkB38;
    u8 padB3C[0xb4c - 0xb3c];
    s32 unkB4C;
    s32 unkB50;
} UnkStruct_02304a24;

typedef struct UnkStruct_02304a24_p {
    u8 pad[0x40];
    s32 unk40;
    u8 pad44[0x31c - 0x44];
    s32 unk31C;
    s32 unk320;
} UnkStruct_02304a24_p;

extern UnkStruct_02304a24 *data_ov001_0231f54c;
extern u32 gRandSeed;
extern s16 data_0206e470[];
extern s32 data_ov001_02316ef4[];
extern s32 data_ov001_02316f30[];
extern s32 data_ov001_02316ecc[];
extern u32 gSystemFlags;
extern u16 gDisplayMain[];

UnkStruct_02304a24_p *func_ov001_022ffce8(void);
s32 func_02002094(s32 n, s32 d);
s32 func_0200203c(s32 n, s32 d);

static inline u16 Rand(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return (u16)(gRandSeed >> 16);
}

static inline s16 Sin(u16 a) {
    return data_0206e470[((a & 0xffff) >> 4) * 2];
}

static inline s32 FX_Mul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

void func_ov001_02304a24(void) {
    s32 type;
    u16 phase;
    u16 x;
    u16 y;
    UnkStruct_02304a24 *g;
    s32 amp;
    s16 *w;
    s32 i;
    s32 ang;
    s32 period;
    s32 a;
    s32 b;
    s32 t;
    s32 s;
    s32 k;
    s32 j;
    u16 *dst;
    s16 *src;

    if (data_ov001_0231f54c->flags & 1)
        return;
    type = func_ov001_022ffce8()->unk40;
    x = (func_02002094((func_ov001_022ffce8()->unk31C >> 12) - 0x80, 3) + (data_ov001_0231f54c->unk588 >> 16)) & 0xff;
    y = func_02002094(((func_ov001_022ffce8()->unk320 >> 12) - 0x60) * 0x38, 0xc0) & 0xff;
    g = data_ov001_0231f54c;
    if (g->unkB4C > 0) {
        g->unkB4C--;
        x += func_0200203c(Rand(), data_ov001_0231f54c->unkB50) - (data_ov001_0231f54c->unkB50 >> 1);
        y += func_0200203c(Rand(), data_ov001_0231f54c->unkB50) - (data_ov001_0231f54c->unkB50 >> 1);
        g = data_ov001_0231f54c;
    }
    g->unkB34 += 0x800;
    g = data_ov001_0231f54c;
    phase = g->unkB34 >> 12;
    if (phase + 0x100 - g->unkB30 > 0) {
        amp = (Rand() & 0x7ff) + 0x1000;
        k = func_0200203c(g->unkB30 + 1, 0x140);
        w = &data_ov001_0231f54c->wave[k];
        ang = (u16)((func_02002094(k, 0x20) & 1) << 15);
        for (i = 0; i < 0x20; i++, w++) {
            *w = FX_Mul(Sin(ang), amp) >> 9;
            ang += 0x400;
        }
        data_ov001_0231f54c->unkB30 += 0x20;
    }
    if (data_ov001_0231f54c->unkB34 >= 0x140000) {
        data_ov001_0231f54c->unkB34 -= 0x140000;
        data_ov001_0231f54c->unkB30 -= 0x140;
    }
    data_ov001_0231f54c->unkB2C ^= 1;
    period = data_ov001_02316ef4[type];
    a = data_ov001_02316f30[type];
    b = data_ov001_02316ecc[type];
    data_ov001_0231f54c->unkB38++;
    if (data_ov001_0231f54c->unkB38 > period * 2)
        data_ov001_0231f54c->unkB38 = 0;
    t = data_ov001_0231f54c->unkB38;
    if (t >= period)
        t = period * 2 - t;
    s = func_02002094(a * t + b * (period - t), period);
    g = data_ov001_0231f54c;
    dst = g->hdma[g->unkB2C];
    k = (phase + y) & 0xff;
    src = &g->wave[k];
    j = 0;
    for (; j < 0xc0 && k < 0x140; j++, k++, dst++, src++) {
        *dst = x + (FX_Mul(*src << 12, s) >> 12);
    }
    if (j < 0xc0) {
        src = data_ov001_0231f54c->wave;
        for (; j < 0xc0; j++) {
            *dst = x + (FX_Mul(*src << 12, s) >> 12);
            src++;
            dst++;
        }
    }
    gSystemFlags |= 0x2000;
    gDisplayMain[3] = y;
}
