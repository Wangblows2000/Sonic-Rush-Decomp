#include <nitro/types.h>

typedef struct UnkStruct_022f240c_o {
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
} UnkStruct_022f240c_o;

typedef struct UnkStruct_022f240c_q {
    u8 pad0[0x74];
    void (*unk74)(void);
    u8 pad78[0x84 - 0x78];
    UnkStruct_022f240c_o *unk84;
} UnkStruct_022f240c_q;

typedef struct UnkStruct_022f240c_s {
    u8 pad0[0x18];
    UnkStruct_022f240c_q *unk18;
} UnkStruct_022f240c_s;

typedef struct UnkStruct_022f240c {
    u8 pad0[0x18];
    s32 unk18;
    s32 unk1C;
    u8 pad20[0x50 - 0x20];
    s32 unk50;
    s32 unk54;
    u8 pad58[0x74 - 0x58];
    void (*unk74)(void);
    u8 pad78[0x84 - 0x78];
    UnkStruct_022f240c_o *unk84;
    u8 pad88[0xb8 - 0x88];
    u32 unkB8;
    u8 padBC[0x160 - 0xbc];
    u32 unk160;
    u8 pad164[0x1c8 - 0x164];
    s32 unk1C8;
    s32 unk1CC;
    u8 pad1D0[0x1dc - 0x1d0];
    UnkStruct_022f240c_s *unk1DC;
} UnkStruct_022f240c;

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

typedef struct UnkStruct_022f240c_g {
    u8 pad0[0x308];
    s32 unk308;
    s32 unk30C;
} UnkStruct_022f240c_g;

typedef struct UnkStruct_022f240c_p {
    u8 pad0[0x18];
    s32 unk18;
    s32 unk1C;
    u8 pad20[0x74 - 0x20];
    void (*unk74)(void);
    u8 pad78[0x84 - 0x78];
    UnkStruct_022f240c_o *unk84;
    u8 pad88[0x1cc - 0x88];
    s32 unk1CC;
    s16 unk1D0;
    u8 pad1D2[2];
    s16 unk1D4;
    u8 pad1D6[0x1dc - 0x1d6];
    UnkStruct_022f240c_s *unk1DC;
} UnkStruct_022f240c_p;

void ExtraBossObj_State_Unk_ov001_022f22ac(void);
UnkStruct_022f240c_g *ExtraBoss_GetWork(void);
s32 FX_DivS32(s32 n, s32 d);
s32 FX_Div(s32 n, s32 d);
void ExtraBoss_Unk_ov001_022fa990(s32 a, s32 b, s32 c, s32 d, s16 e);

void ExtraBossObj_State_Unk_ov001_022f240c(UnkStruct_022f240c_p *p) {
    UnkStruct_022f240c_o *a = p->unk84;
    UnkStruct_022f240c_o *b;

    if (!(a->unk10C & 2))
        return;
    b = p->unk1DC->unk18->unk84;
    p->unk74 = ExtraBossObj_State_Unk_ov001_022f22ac;
    Model_SetAnimation(a, 0, data_ov001_0231f068, data_ov001_02316840[0x18], 0);
    Model_SetAnimation(a, 4, data_ov001_0231f074, data_ov001_02316840[0x19], 0);
    a->unk114 = FX_DivS32(ExtraBoss_GetWork()->unk308 * 0x1333 + (ExtraBoss_GetWork()->unk30C - ExtraBoss_GetWork()->unk308) * 0x1800, ExtraBoss_GetWork()->unk30C);
    Model_SetAnimation(b, 0, data_ov001_0231f068, data_ov001_02316840[0x1a], 0);
    Model_SetAnimation(b, 4, data_ov001_0231f074, data_ov001_02316840[0x1b], 0);
    b->unk114 = a->unk114;
    p->unk1CC = p->unk18;
    p->unk1D0 = 0;
    p->unk1D4 = FX_Div(0x88, a->unk114);
    ExtraBoss_Unk_ov001_022fa990(0, p->unk18 - 0x46000, p->unk1C, 0x28000, 0);
}
