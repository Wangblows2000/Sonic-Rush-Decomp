#include <nitro/types.h>

typedef struct UnkStruct_022f2f14_m33 {
    s32 m[9];
} UnkStruct_022f2f14_m33;

typedef struct UnkStruct_022f2f14_a {
    u8 pad0[0x24];
    UnkStruct_022f2f14_m33 mtx;
    s32 unk48;
    s32 unk4C;
    s32 unk50;
} UnkStruct_022f2f14_a;

typedef struct UnkStruct_022f2f14_c {
    u8 pad0[0x18];
    s32 unk18;
    s32 unk1C;
    u8 pad20[0x28 - 0x20];
    s32 unk28;
    s32 unk2C;
    s32 unk30;
} UnkStruct_022f2f14_c;

typedef struct UnkStruct_022f2f14_s {
    u8 pad0[0x14];
    u16 unk14;
    u16 unk16;
    u8 pad18[0x13c - 0x18];
    s32 unk13C;
    s32 unk140;
    s32 unk144;
    u8 pad148[0x16c - 0x148];
    u8 unk16C[0x190 - 0x16c];
    s32 unk190;
    s32 unk194;
    s32 unk198;
    u8 unk19C[4];
} UnkStruct_022f2f14_s;

typedef struct UnkStruct_022f2f14_p {
    u8 pad0[0x1dc];
    UnkStruct_022f2f14_s *unk1DC;
} UnkStruct_022f2f14_p;

typedef struct UnkStruct_022f2f14 {
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
    u8 pad42[0x7c - 0x42];
    UnkStruct_022f2f14_p *unk7C;
    u8 pad80[4];
    UnkStruct_022f2f14_a *unk84;
    u8 pad88[0xa0 - 0x88];
    UnkStruct_022f2f14_c *unkA0;
    UnkStruct_022f2f14_c *unkA4;
} UnkStruct_022f2f14;

extern s32 data_ov001_0231f524;
extern s32 data_ov001_0231f520;
extern s16 data_0206e470[];

void MTX_RotX33_(void *m, s32 s, s32 c);
void MTX_RotY33_(void *m, s32 s, s32 c);
void MTX_RotZ33_(void *m, s32 s, s32 c);
void MTX_Concat33(void *a, void *b, void *c);
void Model_Animate(void *p);
void Model_Draw(void *p);
void NNS_G3dGeBufferOP_N(u32 op, const u32 *args, u32 n);
void NNS_G3dGetCurrentMtx(void *mtx, void *p);
UnkStruct_022f2f14_c *ExtraBoss_GetPlayer(void);

#define SIN(a) data_0206e470[(((a) & 0xffff) >> 4) * 2]
#define COS(a) data_0206e470[(((a) & 0xffff) >> 4) * 2 + 1]

void ExtraBossObj_Draw_HandCatch(UnkStruct_022f2f14 *w) {
    u32 c0;
    u32 c1;
    UnkStruct_022f2f14_m33 m;
    UnkStruct_022f2f14_s *s = w->unk7C->unk1DC;
    UnkStruct_022f2f14_a *a = w->unk84;
    UnkStruct_022f2f14_c *c;
    UnkStruct_022f2f14_c *t;

    if (s->unk14 == 0) {
        w->unk18 = s->unk13C;
        w->unk1C = -s->unk140;
        w->unk20 = s->unk144;
    }
    a->unk48 = w->unk18 + w->unk30;
    a->unk4C = -(w->unk1C + w->unk34);
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
    NNS_G3dGetCurrentMtx(s->unk16C, s->unk19C);
    c = w->unkA0;
    c->unk28 = s->unk190 - data_ov001_0231f524;
    c->unk2C = -(s->unk194 + data_ov001_0231f520);
    c->unk30 = s->unk198;
    c = w->unkA4;
    c->unk28 = s->unk190 - data_ov001_0231f524;
    c->unk2C = -(s->unk194 + data_ov001_0231f520);
    c->unk30 = s->unk198;
    if (s->unk16 == 0)
        return;
    t = ExtraBoss_GetPlayer();
    t->unk18 = s->unk190 - data_ov001_0231f524;
    t->unk1C = -(s->unk194 + data_ov001_0231f520);
}
