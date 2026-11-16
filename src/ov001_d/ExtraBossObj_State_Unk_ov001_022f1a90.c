#include <nitro/types.h>

typedef struct UnkStruct_022f1a90_o {
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
} UnkStruct_022f1a90_o;

typedef struct UnkStruct_022f1a90_q {
    u8 pad0[0x74];
    void (*unk74)(void);
    u8 pad78[0x84 - 0x78];
    UnkStruct_022f1a90_o *unk84;
} UnkStruct_022f1a90_q;

typedef struct UnkStruct_022f1a90_s {
    u8 pad0[0x18];
    UnkStruct_022f1a90_q *unk18;
} UnkStruct_022f1a90_s;

typedef struct UnkStruct_022f1a90 {
    u8 pad0[0x18];
    s32 unk18;
    s32 unk1C;
    u8 pad20[0x50 - 0x20];
    s32 unk50;
    s32 unk54;
    u8 pad58[0x74 - 0x58];
    void (*unk74)(void);
    u8 pad78[0x84 - 0x78];
    UnkStruct_022f1a90_o *unk84;
    u8 pad88[0xb8 - 0x88];
    u32 unkB8;
    u8 padBC[0x160 - 0xbc];
    u32 unk160;
    u8 pad164[0x1c8 - 0x164];
    s32 unk1C8;
    s32 unk1CC;
    u8 pad1D0[0x1dc - 0x1d0];
    UnkStruct_022f1a90_s *unk1DC;
} UnkStruct_022f1a90;

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

void ExtraBossObj_State_Unk_ov001_022f18e4(void);

void ExtraBossObj_State_Unk_ov001_022f1a90(UnkStruct_022f1a90 *p) {
    UnkStruct_022f1a90_o *a;
    UnkStruct_022f1a90_o *b;

    if (p->unk18 + p->unk50 < 0x162000)
        return;
    a = p->unk84;
    b = p->unk1DC->unk18->unk84;
    p->unk18 = 0x162000;
    p->unk1C = 0xc0000;
    p->unk54 = 0;
    p->unk50 = p->unk54;
    p->unk74 = ExtraBossObj_State_Unk_ov001_022f18e4;
    p->unkB8 |= 0x200;
    a->unk10C |= 0x40;
    Model_SetAnimation(a, 0, data_ov001_0231f068, data_ov001_02316840[0x30], 0);
    Model_SetAnimation(a, 4, data_ov001_0231f074, data_ov001_02316840[0x31], 0);
    a->unk12C = 0x400;
    a->unk10C &= ~4;
    a->unk110 &= ~4;
    a->unk114 = 0x1000;
    b->unk10C |= 0x40;
    Model_SetAnimation(b, 0, data_ov001_0231f068, data_ov001_02316840[0x32], 0);
    Model_SetAnimation(b, 4, data_ov001_0231f074, data_ov001_02316840[0x33], 0);
    b->unk12C = 0x400;
    b->unk10C &= ~4;
    b->unk110 &= ~4;
    b->unk114 = 0x1000;
}
