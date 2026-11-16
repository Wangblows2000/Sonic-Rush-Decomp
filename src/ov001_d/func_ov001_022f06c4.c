#include <nitro/types.h>

typedef struct UnkStruct_022f06c4_o {
    u8 pad0[0xe4];
    s32 *unkE4;
    u8 padE8[0x10c - 0xe8];
    u8 unk10C;
    u8 pad10D[3];
    u8 unk110;
    u8 pad111[3];
    s32 unk114;
} UnkStruct_022f06c4_o;

typedef struct UnkStruct_022f06c4_q {
    u8 pad0[0x84];
    UnkStruct_022f06c4_o *unk84;
} UnkStruct_022f06c4_q;

typedef struct UnkStruct_022f06c4_s {
    u8 pad0[0x18];
    UnkStruct_022f06c4_q *unk18;
} UnkStruct_022f06c4_s;

typedef struct UnkStruct_022f06c4 {
    u8 pad0[0x18];
    s32 unk18;
    u8 pad1C[0x74 - 0x1c];
    void (*unk74)(void);
    u8 pad78[0x84 - 0x78];
    UnkStruct_022f06c4_o *unk84;
    u8 pad88[0x1cc - 0x88];
    s16 unk1CC;
    u8 pad1CE[0x1dc - 0x1ce];
    UnkStruct_022f06c4_s *unk1DC;
} UnkStruct_022f06c4;

typedef struct UnkStruct_022f06c4_t {
    u8 pad0[0x98];
    u16 unk98;
    u16 unk9A;
    u16 unk9C;
    u16 unk9E;
} UnkStruct_022f06c4_t;

extern void *data_ov001_0231f068;
extern void *data_ov001_0231f074;
extern UnkStruct_022f06c4_t data_ov001_02316840;
extern u32 gRandSeed;

void func_ov001_022f0548(void);
void Model_SetAnimation(void *p, s32 a, void *b, s32 c, s32 d);
u8 *func_ov001_022ffce8(void);
void Sound_PlaySeqArc(void *p, u32 a, u32 b);
void func_020366f4(void *p, u32 a, u32 b);

static inline s32 FX_Mul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

void func_ov001_022f06c4(UnkStruct_022f06c4 *p) {
    UnkStruct_022f06c4_o *a;
    UnkStruct_022f06c4_o *b;
    s32 v;
    s32 acc;
    s32 i;

    p->unk1CC += 0x44;
    v = p->unk1CC;
    if (v >= 0x1000) {
        a = p->unk84;
        b = p->unk1DC->unk18->unk84;
        p->unk74 = func_ov001_022f0548;
        Model_SetAnimation(a, 0, data_ov001_0231f068, data_ov001_02316840.unk98, 0);
        Model_SetAnimation(a, 4, data_ov001_0231f074, data_ov001_02316840.unk9A, 0);
        a->unk10C &= ~4;
        a->unk110 &= ~4;
        a->unk114 = 0x1000;
        Model_SetAnimation(b, 0, data_ov001_0231f068, data_ov001_02316840.unk9C, 0);
        Model_SetAnimation(b, 4, data_ov001_0231f074, data_ov001_02316840.unk9E, 0);
        b->unk10C &= ~4;
        b->unk110 &= ~4;
        b->unk114 = 0x1000;
        return;
    }
    i = 2;
    acc = -0xc8000;
    do {
        acc += FX_Mul(0xc0000 - acc, v);
    } while (i-- != 0);
    p->unk18 = acc;
}
