#include <nitro/types.h>

typedef struct UnkStruct_022f3164_m33 {
    s32 m[9];
} UnkStruct_022f3164_m33;

typedef struct UnkStruct_022f3164_a {
    u8 pad0[0x24];
    UnkStruct_022f3164_m33 mtx;
    s32 unk48;
    s32 unk4C;
    s32 unk50;
} UnkStruct_022f3164_a;

typedef struct UnkStruct_022f3164_c {
    u8 pad0[0x28];
    s32 unk28;
    s32 unk2C;
    s32 unk30;
} UnkStruct_022f3164_c;

typedef struct UnkStruct_022f3164_s {
    u8 pad0[0x1c];
    u8 unk1C[0x30];
    u8 unk4C[0x24];
    u8 unk70[0x30];
    u8 unkA0[0x24];
    u8 unkC4[0x30];
    u8 unkF4[0x24];
    u8 unk118[0x30];
    u8 unk148[0x1c0 - 0x148];
    s32 unk1C0;
} UnkStruct_022f3164_s;

typedef struct UnkStruct_022f3164 {
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
    u8 pad42[0x84 - 0x42];
    UnkStruct_022f3164_a *unk84;
    u8 pad88[0x9c - 0x88];
    UnkStruct_022f3164_c *unk9C;
    UnkStruct_022f3164_c *unkA0;
    UnkStruct_022f3164_c *unkA4;
    UnkStruct_022f3164_c *unkA8;
    u8 padAC[0xb8 - 0xac];
    u32 unkB8;
    u8 padBC[0x1dc - 0xbc];
    UnkStruct_022f3164_s *unk1DC;
} UnkStruct_022f3164;

extern s32 data_ov001_0231f524;
extern s32 data_ov001_0231f520;
extern s16 data_0206e470[];
extern s32 data_ov001_0231f070;
extern void *data_ov001_0231f528;
extern s32 data_ov001_0231d408[];
extern u16 data_ov001_0231670c[];
extern u16 data_ov001_02316710[];
extern u16 data_ov001_02316714[];

void MTX_RotX33_(void *m, s32 s, s32 c);
void MTX_RotY33_(void *m, s32 s, s32 c);
void MTX_RotZ33_(void *m, s32 s, s32 c);
void MTX_Concat33(void *a, void *b, void *c);
void Model_Animate(void *p);
void Model_Draw(void *p);
void NNS_G3dGeBufferOP_N(u32 op, const u32 *args, u32 n);
void NNS_G3dGetCurrentMtx(void *mtx, void *p);
void func_ov001_022ffb9c(s32 a, void *b, s32 *c, u16 *d, u16 *e, s32 n);

#define SIN(a) data_0206e470[(((a) & 0xffff) >> 4) * 2]
#define COS(a) data_0206e470[(((a) & 0xffff) >> 4) * 2 + 1]

void func_ov001_022f3164(UnkStruct_022f3164 *w) {
    u32 c0;
    u32 c1;
    u32 c2;
    u32 c3;
    u32 c4;
    UnkStruct_022f3164_m33 m;
    UnkStruct_022f3164_s *s = w->unk1DC;
    UnkStruct_022f3164_a *a = w->unk84;
    s32 t;
    UnkStruct_022f3164_c *c;

    a->unk48 = data_ov001_0231f524 + (w->unk18 + w->unk30);
    a->unk4C = -(data_ov001_0231f520 + (w->unk1C + w->unk34));
    a->unk50 = w->unk20 + w->unk38;
    MTX_RotX33_(&a->mtx, SIN(w->unk3C), COS(w->unk3C));
    MTX_RotY33_(&m, SIN(w->unk3E), COS(w->unk3E));
    MTX_Concat33(&a->mtx, &m, &a->mtx);
    MTX_RotZ33_(&m, SIN(w->unk40), COS(w->unk40));
    MTX_Concat33(&a->mtx, &m, &a->mtx);
    if (!(w->unk14 & 0x10000))
        Model_Animate(a);
    Model_Draw(a);
    c0 = 2;
    NNS_G3dGeBufferOP_N(0x10, &c0, 1);
    c1 = 0x1d;
    NNS_G3dGeBufferOP_N(0x14, &c1, 1);
    NNS_G3dGetCurrentMtx(s->unk1C, s->unk4C);
    c2 = 0x1b;
    NNS_G3dGeBufferOP_N(0x14, &c2, 1);
    NNS_G3dGetCurrentMtx(s->unk70, s->unkA0);
    c3 = 0x1c;
    NNS_G3dGeBufferOP_N(0x14, &c3, 1);
    NNS_G3dGetCurrentMtx(s->unkC4, s->unkF4);
    c4 = 0x1e;
    NNS_G3dGeBufferOP_N(0x14, &c4, 1);
    NNS_G3dGetCurrentMtx(s->unk118, s->unk148);
    c = w->unk9C;
    c->unk28 = *(s32 *)(s->unk4C + 0x48) - data_ov001_0231f524;
    c->unk2C = -(*(s32 *)(s->unk4C + 0x4c) + data_ov001_0231f520);
    c->unk30 = *(s32 *)(s->unk4C + 0x50);
    c = w->unkA0;
    c->unk28 = *(s32 *)(s->unk1C + 0x24) - data_ov001_0231f524;
    c->unk2C = -(*(s32 *)(s->unk1C + 0x28) + data_ov001_0231f520);
    c->unk30 = *(s32 *)(s->unk1C + 0x2c);
    c = w->unkA4;
    c->unk28 = *(s32 *)(s->unkC4 + 0x24) - data_ov001_0231f524;
    c->unk2C = -(*(s32 *)(s->unkC4 + 0x28) + data_ov001_0231f520);
    c->unk30 = *(s32 *)(s->unkC4 + 0x2c);
    c = w->unkA8;
    c->unk28 = *(s32 *)(s->unkC4 + 0x24) - data_ov001_0231f524;
    c->unk2C = -(*(s32 *)(s->unkC4 + 0x28) + data_ov001_0231f520);
    c->unk30 = *(s32 *)(s->unkC4 + 0x2c);
    if (!(w->unkB8 & 0x200)) {
        s->unk1C0++;
        t = s->unk1C0 & 7;
        if (t == 1)
            func_ov001_022ffb9c(data_ov001_0231f070, data_ov001_0231f528, data_ov001_0231d408, data_ov001_0231670c, data_ov001_02316714, 2);
        else if (t == 5)
            func_ov001_022ffb9c(data_ov001_0231f070, data_ov001_0231f528, data_ov001_0231d408, data_ov001_02316710, data_ov001_02316714, 2);
    } else if (s->unk1C0 != 0) {
        func_ov001_022ffb9c(data_ov001_0231f070, data_ov001_0231f528, data_ov001_0231d408, data_ov001_02316710, data_ov001_02316714, 2);
        s->unk1C0 = 0;
    }
}
