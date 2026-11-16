#include <nitro/types.h>

typedef struct UnkStruct_023122d8_e {
    u16 unk0;
    u16 unk2;
    u16 unk4;
    u16 unk6;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
    s32 unk2C;
    void *unk30;
} UnkStruct_023122d8_e;

typedef struct UnkStruct_023122d8_a {
    u8 pad0[0x10c];
    u8 unk10C;
    u8 pad10D[0x114 - 0x10d];
    s32 unk114;
    u8 pad118[0x12e - 0x118];
    u16 unk12E;
} UnkStruct_023122d8_a;

typedef struct UnkStruct_023122d8_g {
    u8 pad0[4];
    u8 unk4[0x13c - 4];
    void *unk13C;
    void *unk140;
    void *unk144;
} UnkStruct_023122d8_g;

typedef struct UnkStruct_023122d8_t {
    u32 v[3];
} UnkStruct_023122d8_t;

typedef struct UnkStruct_023122d8 {
    u8 pad0[0x16];
    u16 unk16;
    s32 unk18;
    s32 unk1C;
    u8 pad20[8];
    s32 unk28;
    u8 pad2C[0x18];
    s32 velx, vely, velz;
    s32 accx, accy, accz;
    u8 pad5C[0xac - 0x5c];
    UnkStruct_023122d8_a *unkAC;
    u8 padB0[4];
    void *unkB4;
    s32 unkB8;
    void *unkBC;
    s32 unkC0;
    u8 padC4[0x12c - 0xc4];
    void *unk12C;
    s32 unk130;
    s32 unk134;
    u8 pad138[8];
    s32 unk140;
    s32 unk144;
    u8 pad148[0x158 - 0x148];
    s32 unk158;
    s32 unk15C;
    u8 pad160[0x1f0 - 0x160];
    s32 unk1F0;
    u8 pad1F4[0x228 - 0x1f4];
    s32 unk228;
    void *unk22C;
} UnkStruct_023122d8;

extern UnkStruct_023122d8_e data_ov001_02317904[];
extern s16 data_0206e470[];
extern s32 data_ov001_023178d8[];
extern UnkStruct_023122d8_g *data_ov001_0231f694;
extern u8 data_ov001_0231eed0[];
extern u8 data_ov001_0231eee0[];
extern u8 data_ov001_0231eec0[];
extern u32 gRandSeed;
extern UnkStruct_023122d8_t data_ov001_0231ee9c;
extern u32 data_ov001_023178e4[];

void Model_SetResource(void *s, void *res, u32 idx);
void Model_SetAnimation(void *p, s32 a, void *b, s32 c, u32 d);
void func_ov001_02312aac(UnkStruct_023122d8 *w);
void func_ov001_023120c8(UnkStruct_023122d8 *w);
void Sound_PlaySeqArc(u32 a, u32 b, u32 c);

#define SIN(a) data_0206e470[(((a) & 0xffff) >> 4) * 2]

static inline s32 FX_Mul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

static inline u16 Rand(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return (u16)(gRandSeed >> 16);
}

void func_ov001_023122d8(UnkStruct_023122d8 *w, s32 state) {
    UnkStruct_023122d8_e *e = &data_ov001_02317904[state];
    UnkStruct_023122d8_e *cur = &data_ov001_02317904[w->unk228];
    s32 neg = -1;

    switch (e->unk14) {
    case 0:
        w->velx = e->unk18;
        w->vely = e->unk20;
        w->velz = e->unk28;
        w->accx = e->unk1C;
        w->accy = e->unk24;
        w->accz = e->unk2C;
        if (w->unk16 >= 0x8000) {
            w->velx *= neg;
            w->accx *= neg;
        }
        break;
    case 2:
        w->accx = e->unk1C;
        w->accy = e->unk24;
        w->accz = e->unk2C;
        if (w->unk16 >= 0x8000)
            w->accx *= neg;
        break;
    case 3:
        w->vely = 0;
        w->velx = 0;
        w->velz = 0;
        w->accx = 0;
        w->accy = 0;
        w->accz = 0;
        break;
    case 4:
        w->vely = w->velx * e->unk20;
        w->velx = e->unk18;
        w->velz = e->unk28;
        w->accx = e->unk1C;
        w->accy = e->unk24;
        w->accz = e->unk2C;
        if (w->unk16 >= 0x8000) {
            w->velx *= neg;
            w->accx *= neg;
            w->vely *= neg;
        }
        break;
    case 5:
        w->velx = w->vely / e->unk18;
        w->vely = e->unk20;
        w->velz = e->unk28;
        w->accx = e->unk1C;
        w->accy = e->unk24;
        w->accz = e->unk2C;
        if (w->unk16 >= 0x8000) {
            w->velx *= neg;
            w->accx *= neg;
        }
        break;
    case 6:
        w->velx = FX_Mul(SIN(w->unk16), e->unk18);
        w->vely = e->unk20;
        w->velz = e->unk28;
        w->accx = FX_Mul(SIN(w->unk16), e->unk1C);
        w->accy = e->unk24;
        w->accz = e->unk2C;
        break;
    case 7:
        w->velx = e->unk18;
        w->vely = e->unk20 + -w->unk28 / 300;
        w->velz = e->unk28;
        w->accx = e->unk1C;
        w->accy = e->unk24;
        w->accz = e->unk2C;
        if (w->unk134 == data_ov001_023178d8[0] || w->unk134 == data_ov001_023178d8[2])
            w->vely += 0x800;
        break;
    }
    if (cur->unk0 != e->unk0) {
        Model_SetResource(w->unkAC, data_ov001_0231f694->unk13C, e->unk0);
        Model_SetAnimation(w->unkAC, 0, data_ov001_0231f694->unk140, e->unk2, 0);
        Model_SetAnimation(data_ov001_0231f694->unk4, 4, data_ov001_0231f694->unk144, e->unk4, 0);
    } else {
        if (cur->unk2 != e->unk2) {
            w->unkAC->unk10C |= 0x40;
            switch (w->unk228) {
            case 3:
                w->unkAC->unk12E = 0xcc;
                break;
            case 0xb:
            case 0xc:
            case 0xd:
                w->unkAC->unk12E = 0x1eb;
                break;
            case 0x15:
                w->unkAC->unk12E = 0x400;
                break;
            default:
                w->unkAC->unk12E = 0x333;
                break;
            }
            Model_SetAnimation(w->unkAC, 0, data_ov001_0231f694->unk140, e->unk2, 0);
        }
        if (cur->unk4 != e->unk4)
            Model_SetAnimation(data_ov001_0231f694->unk4, 4, data_ov001_0231f694->unk144, e->unk4, 0);
    }
    w->unkAC->unk114 = e->unk8;
    if (e->unkC != 0)
        w->unkAC->unk10C |= 4;
    else
        w->unkAC->unk10C &= ~4;
    if (e->unk6 & 1) {
        w->unkBC = NULL;
        w->unkC0 = 0;
    } else {
        w->unkBC = data_ov001_0231eed0;
        w->unkC0 = 1;
    }
    if (e->unk6 & 2) {
        w->unk12C = NULL;
        w->unk130 = 0;
    } else {
        w->unk12C = data_ov001_0231eee0;
        w->unk130 = 1;
    }
    if (e->unk6 & 4) {
        w->unkB4 = data_ov001_0231eec0;
        w->unkB8 = 1;
    } else {
        w->unkB4 = NULL;
        w->unkB8 = 0;
    }
    if (e->unk6 & 0x10) {
        w->unk16 = 0;
        w->unk18 = 0;
        w->unk1C = 0;
        func_ov001_02312aac(w);
    }
    if (!(e->unk6 & 0x800) && w->unk144 > 0)
        func_ov001_023120c8(w);
    if (e->unk6 & 0x1000)
        w->unk1F0 = 1;
    else
        w->unk1F0 = 0;
    if (e->unk6 & 0x20)
        w->unk16 = 0;
    w->unk228 = state;
    w->unk22C = e->unk30;
    if (e->unk10 >= 0) {
        Sound_PlaySeqArc(0, 0x10, e->unk10);
    } else {
        switch (state) {
        case 0xb:
        case 0xc:
        case 0xd: {
            UnkStruct_023122d8_t t = data_ov001_0231ee9c;
            Sound_PlaySeqArc(0, 0x10, data_ov001_023178e4[Rand() % 4]);
            Sound_PlaySeqArc(0, 0x10, t.v[w->unk158]);
            break;
        }
        }
    }
    w->unk140 = 0;
    w->unk15C = 0;
}
