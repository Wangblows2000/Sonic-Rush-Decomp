#include <nitro/types.h>

typedef struct UnkStruct_022cd880_a {
    u8 pad[0xe4];
    s32 *unkE4;
    u8 pad1[0x10c - 0xe8];
    u8 unk10C;
    u8 pad2[0x114 - 0x10d];
    s32 unk114;
    u8 pad3[0x138 - 0x118];
} UnkStruct_022cd880_a;

typedef struct UnkStruct_022cd880_h {
    u8 pad[0xa];
    u16 unkA;
} UnkStruct_022cd880_h;

typedef struct UnkStruct_022cd880 UnkStruct_022cd880;
struct UnkStruct_022cd880 {
    u8 pad[0x68];
    void (*func)(UnkStruct_022cd880 *s);
    u8 pad1[0xa9c - 0x6c];
    UnkStruct_022cd880_a unkA9C;
    UnkStruct_022cd880_a unkBD4;
    UnkStruct_022cd880_a unkD0C;
    u8 pad2[0xf7c - 0xe44];
    s32 unkF7C;
    u8 pad3[0xfdc - 0xf80];
    UnkStruct_022cd880_h unkFDC;
    u8 pad4[0x1008 - 0xfdc - sizeof(UnkStruct_022cd880_h)];
    UnkStruct_022cd880_h unk1008;
    u8 pad5[0x1060 - 0x1008 - sizeof(UnkStruct_022cd880_h)];
    UnkStruct_022cd880_h unk1060;
    u8 pad6[0x1110 - 0x1060 - sizeof(UnkStruct_022cd880_h)];
    UnkStruct_022cd880_h unk1110;
    u8 pad7[0x139c - 0x1110 - sizeof(UnkStruct_022cd880_h)];
    s32 unk139C;
    s32 unk13A0;
    s32 unk13A4;
    u8 pad8[0x1458 - 0x13a8];
    s32 unk1458;
    u16 unk145C;
    u8 pad10[0x1474 - 0x145e];
    void *unk1474;
};

typedef struct UnkStruct_022cd880_c {
    u8 pad[0x18];
    void *unk18;
} UnkStruct_022cd880_c;

typedef struct UnkStruct_022cd880_2 {
    u8 unk0;
} UnkStruct_022cd880_2;

typedef struct UnkStruct_022cd880_7 {
    u8 pad[4];
    s32 unk4;
    u16 unk8;
} UnkStruct_022cd880_7;

extern UnkStruct_022cd880_7 data_ov000_02369ac0;
extern u32 gRandSeed;
extern s32 data_ov000_02362e14[];

void func_ov000_022cff50(void);
void func_ov000_022d0864(UnkStruct_022cd880 *s);
void func_ov000_022d046c(UnkStruct_022cd880 *s);
void Model_SetAnimation(void *p, s32 a, s32 b, s32 c, s32 d);
void func_ov000_022cd1b0(u32 flags);
void func_ov000_022cedd8(UnkStruct_022cd880 *s, s32 a);
void BossEffect_CreateHit(s32 a, s32 x, s32 y, s32 z);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void func_ov000_0234aa80(UnkStruct_022cd880_2 *p);
void func_020537b4(s32 a);
void func_020533e0(s32 a, s32 b);
void Sound_PlaySeq(s32 a, s32 b);
void func_020366f4(s32 a, s32 b, s32 c);

static inline u16 Rand(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return (u16)(gRandSeed >> 16);
}

void func_ov000_022cd880(UnkStruct_022cd880_c *a, UnkStruct_022cd880_c *b) {
    UnkStruct_022cd880 *s = a->unk18;
    UnkStruct_022cd880_a *x = &s->unkA9C;
    UnkStruct_022cd880_a *y = &s->unkBD4;
    UnkStruct_022cd880_a *z = &s->unkD0C;
    UnkStruct_022cd880_2 *p;
    s32 v;
    if (data_ov000_02369ac0.unk8 != 0) {
    data_ov000_02369ac0.unk8--;
    s->unk1008.unkA &= ~4;
    s->unk1008.unkA |= 0x1000;
    s->unk1110.unkA &= ~4;
    s->unk1110.unkA |= 0x1000;
    s->unk1060.unkA &= ~4;
    s->unk1060.unkA |= 0x1000;
    s->unkFDC.unkA &= ~4;
    s->unkFDC.unkA |= 0x1000;
    if (data_ov000_02369ac0.unk8 != 0) {
    v = *y->unkE4;
    if (s->func == func_ov000_022d0864)
        v = 0x12d000 - v;
    s->func = func_ov000_022d046c;
    if (s->unk145C) {
        Model_SetAnimation(x, 0, s->unkF7C, 0x1c, 0);
        Model_SetAnimation(y, 0, s->unkF7C, 0x37, 0);
        Model_SetAnimation(z, 0, s->unkF7C, 0x58, 0);
    } else {
        Model_SetAnimation(x, 0, s->unkF7C, 0x16, 0);
        Model_SetAnimation(y, 0, s->unkF7C, 0x33, 0);
        Model_SetAnimation(z, 0, s->unkF7C, 0x52, 0);
    }
    x->unk10C &= ~4;
    y->unk10C &= ~4;
    z->unk10C &= ~4;
    x->unk114 = 0x1000;
    y->unk114 = 0x4000;
    z->unk114 = 0x1000;
    *y->unkE4 = v;
    func_ov000_022cd1b0(0);
    s->unk1474 = NULL;
    data_ov000_02369ac0.unk4 = 0xb;
    func_ov000_022cedd8(s, 0);
    BossEffect_CreateHit(0, s->unk139C >> 4, (-s->unk13A0 >> 4) - 0x3200, s->unk13A4 >> 4);
    Sound_PlaySeqArc(s->unk1458, 0, 0xa4);
    p = b->unk18;
    if (p != NULL && p->unk0 == 1)
        func_ov000_0234aa80(p);
    if (data_ov000_02369ac0.unk8 == 2) {
        func_020537b4(0xb);
        func_020533e0(0x12, 0xf0);
        Sound_PlaySeq(0, 0xf);
    } else {
        func_020537b4(0xb);
        func_020533e0(0x10, 0xf0);
    }
    Sound_PlaySeqArc(0, 0, 0x7b);
    func_020366f4(0, 0, data_ov000_02362e14[Rand() & 1]);
    } else {
        func_ov000_022cff50();
    }
    } else {
        func_ov000_022cff50();
    }
}
