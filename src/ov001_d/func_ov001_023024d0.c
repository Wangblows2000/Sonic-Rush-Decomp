#include <nitro/types.h>

typedef struct UnkStruct_023024d0_o {
    u8 pad0[0xe4];
    s32 *unkE4;
    u8 padE8[0x10c - 0xe8];
    u8 unk10C;
    u8 pad10D[3];
    u8 unk110;
    u8 pad111[3];
    s32 unk114;
    s32 unk118;
    u8 pad11C[0x12c - 0x11c];
    s16 unk12C;
} UnkStruct_023024d0_o;

typedef struct UnkStruct_023024d0_q {
    u8 pad0[0x74];
    void (*unk74)(void);
    u8 pad78[0x84 - 0x78];
    UnkStruct_023024d0_o *unk84;
} UnkStruct_023024d0_q;

typedef struct UnkStruct_023024d0_s {
    u8 pad0[0x18];
    UnkStruct_023024d0_q *unk18;
} UnkStruct_023024d0_s;

typedef struct UnkStruct_023024d0 {
    u8 pad0[0x18];
    s32 unk18;
    s32 unk1C;
    u8 pad20[0x50 - 0x20];
    s32 unk50;
    s32 unk54;
    u8 pad58[0x74 - 0x58];
    void (*unk74)(void);
    u8 pad78[0x84 - 0x78];
    UnkStruct_023024d0_o *unk84;
    u8 pad88[0xb8 - 0x88];
    u32 unkB8;
    u8 padBC[0x160 - 0xbc];
    u32 unk160;
    u8 pad164[0x1c8 - 0x164];
    s32 unk1C8;
    s32 unk1CC;
    u8 pad1D0[0x1dc - 0x1d0];
    UnkStruct_023024d0_s *unk1DC;
} UnkStruct_023024d0;

extern void *data_ov001_0231f068;
extern void *data_ov001_0231f074;
extern u16 data_ov001_02316840[];
extern u32 gRandSeed;

void Model_SetAnimation(void *p, s32 a, void *b, s32 c, s32 d);

static inline u16 Rand(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return gRandSeed >> 16;
}

static inline s32 FX_Mul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

typedef struct { s32 x, y, z; } Vec_023024d0;
typedef struct { s16 x, y, z; } SVec_023024d0;

typedef struct UnkStruct_023024d0_e {
    u16 idx;
    u16 anim0;
    u16 anim1;
    u16 flags;
    Vec_023024d0 v8;
    Vec_023024d0 v14;
    SVec_023024d0 a20;
    SVec_023024d0 a26;
    u8 pad2C[0x34 - 0x2c];
    void (*fn)(void);
} UnkStruct_023024d0_e;

typedef struct UnkStruct_023024d0_s2 {
    u8 pad0[4];
    s32 unk4;
    u8 pad8[4];
    UnkStruct_023024d0_e *unkC;
} UnkStruct_023024d0_s2;

typedef struct UnkStruct_023024d0_p {
    u8 pad0[0x3c];
    s16 unk3C;
    s16 unk3E;
    s16 unk40;
    u8 pad42[0x50 - 0x42];
    Vec_023024d0 v50;
    Vec_023024d0 v5C;
    SVec_023024d0 a68;
    SVec_023024d0 a6E;
    void (*unk74)(void);
    u8 pad78[0x84 - 0x78];
    UnkStruct_023024d0_o *unk84;
    UnkStruct_023024d0_o *objs[1];
    u8 pad8C[0x1dc - 0x8c];
    UnkStruct_023024d0_s2 *unk1DC;
} UnkStruct_023024d0_p;

extern void *data_ov001_0231f534;
extern void *data_ov001_0231f53c;

void func_ov001_023024d0(UnkStruct_023024d0_p *p, s32 n) {
    UnkStruct_023024d0_s2 *s = p->unk1DC;
    UnkStruct_023024d0_e *e = &s->unkC[n];

    p->unk84 = p->objs[s->unkC[n].idx];
    if (e->anim0 != 0xffff) {
        Model_SetAnimation(p->unk84, 0, data_ov001_0231f534, e->anim0, 0);
        p->unk84->unk10C &= ~4;
        if (e->flags & 0x10)
            p->unk84->unk10C |= 4;
    }
    if (e->anim1 != 0xffff) {
        Model_SetAnimation(p->unk84, 4, data_ov001_0231f53c, e->anim1, 0);
        p->unk84->unk110 &= ~4;
        if (e->flags & 0x10)
            p->unk84->unk110 |= 4;
    }
    if (e->flags & 1) {
        p->v50 = e->v8;
        p->v5C = e->v14;
        p->a68 = e->a20;
        p->a6E = e->a26;
    }
    if (e->flags & 2)
        p->unk3C = 0;
    if (e->flags & 4)
        p->unk3E = 0;
    if (e->flags & 6)
        p->unk40 = 0;
    p->unk74 = e->fn;
    s->unk4 = n;
}
