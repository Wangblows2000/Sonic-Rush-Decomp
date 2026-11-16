#include <nitro/types.h>

typedef struct UnkStruct_022d602c_m33 {
    s32 m[9];
} UnkStruct_022d602c_m33;

typedef struct UnkStruct_022d602c_v {
    s32 x;
    s32 y;
    s32 z;
} UnkStruct_022d602c_v;

typedef struct UnkStruct_022d602c_m43 {
    UnkStruct_022d602c_m33 m;
    UnkStruct_022d602c_v t;
} UnkStruct_022d602c_m43;

typedef struct UnkStruct_022d602c_a {
    u8 pad[0xa];
    u8 unkA;
    u8 unkB;
    u8 pad0[0x18 - 0xc];
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    UnkStruct_022d602c_m43 unk24;
    u8 pad1[0x174 - 0x54];
} UnkStruct_022d602c_a;

typedef struct UnkStruct_022d602c {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    u8 pad0[0x2e - 0x18];
    s16 unk2E;
    u8 pad1[0x5c - 0x30];
    u16 unk5C;
    u8 pad2[0x20c - 0x5e];
    u32 unk20C;
    u16 unk210;
    s16 unk212;
    u8 pad3[0x290 - 0x214];
    UnkStruct_022d602c_a unk290;
    UnkStruct_022d602c_a unk404;
} UnkStruct_022d602c;

typedef struct UnkTask_022d602c {
    u8 pad[0x10];
    UnkStruct_022d602c *unk10;
} UnkTask_022d602c;

typedef struct UnkStruct_022d602c_q {
    u8 pad[0x228];
    UnkStruct_022d602c_m43 unk228;
} UnkStruct_022d602c_q;

typedef struct UnkStruct_022d602c_b {
    u8 pad[0x214];
    UnkStruct_022d602c_q *unk214;
} UnkStruct_022d602c_b;

extern UnkTask_022d602c *gTaskCurrent;
extern UnkStruct_022d602c_b *gBoss3;
extern s16 data_0206e470[];

void BossArena_ArcToCirclePos(s32 len, s32 b, s32 a, s32 *outX, s32 *outY, u16 *outAng);
void BossArena_CirclePosToArc(s32 a, s32 b, s32 x, s32 z, s32 *out);
void MTX_RotZ33_(void *m, s32 s, s32 c);
void MTX_RotY33_(void *m, s32 s, s32 c);
void MTX_Concat33(void *a, void *b, void *c);
void Model_Animate(UnkStruct_022d602c_a *a);
void Model_Draw(UnkStruct_022d602c_a *a);

static inline s32 FX_Mul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

void Boss3PartCD_Draw(void) {
    UnkStruct_022d602c *o = gTaskCurrent->unk10;
    UnkStruct_022d602c_a *a = &o->unk290;
    UnkStruct_022d602c_a *b = &o->unk404;
    s32 sc = FX_Mul(o->unk212, 0x34cc);
    u16 ang;
    UnkStruct_022d602c_m33 m1;
    UnkStruct_022d602c_m33 m2;
    if (o->unk20C & 1) {
        a->unkA = 3;
        a->unkB = 0;
        if (!(o->unk20C & 2)) {
            if (o->unk10 < 0x4000)
                o->unk10 += 0x5fe43;
            else if (o->unk10 > 0x63e43)
                o->unk10 -= 0x5fe43;
            BossArena_ArcToCirclePos(0xf42f8, 0x5fe435, o->unk10 << 4, &a->unk24.t.x, &a->unk24.t.z, &ang);
            a->unk24.t.y = -o->unk14 << 4;
            if (!(o->unk5C & 0x10))
                o->unk210 -= o->unk2E;
            MTX_RotZ33_(&a->unk24, data_0206e470[((o->unk210 & 0xffff) >> 4) * 2], data_0206e470[((o->unk210 & 0xffff) >> 4) * 2 + 1]);
            MTX_RotY33_(&m1, data_0206e470[((ang & 0xffff) >> 4) * 2], data_0206e470[((ang & 0xffff) >> 4) * 2 + 1]);
            MTX_Concat33(&a->unk24, &m1, &a->unk24);
        }
    } else {
        a->unkA = 3;
        a->unkB = 0;
        a->unk24 = gBoss3->unk214->unk228;
        BossArena_CirclePosToArc(0xf42f8, 0x5fe435, a->unk24.t.x, a->unk24.t.z, &o->unk10);
        o->unk10 >>= 4;
        BossArena_ArcToCirclePos(0xf42f8, 0x5fe435, o->unk10 << 4, NULL, NULL, &ang);
        MTX_RotZ33_(&a->unk24, data_0206e470[((o->unk210 & 0xffff) >> 4) * 2], data_0206e470[((o->unk210 & 0xffff) >> 4) * 2 + 1]);
        MTX_RotY33_(&m2, data_0206e470[((ang & 0xffff) >> 4) * 2], data_0206e470[((ang & 0xffff) >> 4) * 2 + 1]);
        MTX_Concat33(&a->unk24, &m2, &a->unk24);
    }
    a->unk20 = sc;
    a->unk1C = a->unk20;
    a->unk18 = a->unk1C;
    b->unk24.t.x = a->unk24.t.x;
    b->unk24.t.y = a->unk24.t.y;
    b->unk24.t.z = a->unk24.t.z;
    b->unk24.m = a->unk24.m;
    b->unk20 = sc;
    b->unk1C = b->unk20;
    b->unk18 = b->unk1C;
    if (o->unk5C & 0x20)
        return;
    Model_Animate(a);
    Model_Draw(a);
    if (o->unk20C & 4) {
        Model_Animate(b);
        Model_Draw(b);
    }
}
