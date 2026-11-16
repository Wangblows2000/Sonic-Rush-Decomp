#include <nitro/types.h>

typedef struct UnkStruct_022efea0_o {
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
} UnkStruct_022efea0_o;

typedef struct UnkStruct_022efea0_q {
    u8 pad0[0x74];
    void (*unk74)(void);
    u8 pad78[0x84 - 0x78];
    UnkStruct_022efea0_o *unk84;
} UnkStruct_022efea0_q;

typedef struct UnkStruct_022efea0_s {
    u8 pad0[0x18];
    UnkStruct_022efea0_q *unk18;
} UnkStruct_022efea0_s;

typedef struct UnkStruct_022efea0 {
    u8 pad0[0x18];
    s32 unk18;
    s32 unk1C;
    u8 pad20[0x50 - 0x20];
    s32 unk50;
    s32 unk54;
    u8 pad58[0x74 - 0x58];
    void (*unk74)(void);
    u8 pad78[0x84 - 0x78];
    UnkStruct_022efea0_o *unk84;
    u8 pad88[0xb8 - 0x88];
    u32 unkB8;
    u8 padBC[0x160 - 0xbc];
    u32 unk160;
    u8 pad164[0x1c8 - 0x164];
    s32 unk1C8;
    s32 unk1CC;
    u8 pad1D0[0x1dc - 0x1d0];
    UnkStruct_022efea0_s *unk1DC;
} UnkStruct_022efea0;

extern void *data_ov001_0231f068;
extern void *data_ov001_0231f074;
extern u16 gExtraBossHandBossAnimTable[];
extern u32 gRandSeed;

void Model_SetAnimation(void *p, s32 a, void *b, s32 c, s32 d);

static inline u16 Rand(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return gRandSeed >> 16;
}

static inline s32 FX_Mul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

typedef struct UnkStruct_022efea0_p {
    u8 pad0[0x18];
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    u8 pad24[0x50 - 0x24];
    s32 unk50;
    u8 pad54[0x74 - 0x54];
    void (*unk74)(void);
    u8 pad78[0x84 - 0x78];
    UnkStruct_022efea0_o *unk84;
    u8 pad88[0xb8 - 0x88];
    u32 unkB8;
    u8 padBC[0x1dc - 0xbc];
    UnkStruct_022efea0_s *unk1DC;
} UnkStruct_022efea0_p;

void ExtraBossObj_State_HandBossSuctionReturn(void);
void ExtraBoss_SetCameraParams(s32 a, s32 b, s32 c, s32 d, s32 e);

void ExtraBossObj_State_HandBossSuctionLeave(UnkStruct_022efea0_p *p) {
    UnkStruct_022efea0_o *b;
    UnkStruct_022efea0_o *a;

    if (p->unk18 < 0x248000)
        return;
    a = p->unk84;
    b = p->unk1DC->unk18->unk84;
    p->unk74 = ExtraBossObj_State_HandBossSuctionReturn;
    p->unk50 = -0x1000;
    p->unk20 = 0;
    Model_SetAnimation(a, 0, data_ov001_0231f068, gExtraBossHandBossAnimTable[0x64], 0);
    Model_SetAnimation(a, 4, data_ov001_0231f074, gExtraBossHandBossAnimTable[0x65], 0);
    a->unk10C |= 4;
    a->unk110 |= 4;
    a->unk114 = 0x1000;
    Model_SetAnimation(b, 0, data_ov001_0231f068, gExtraBossHandBossAnimTable[0x66], 0);
    Model_SetAnimation(b, 4, data_ov001_0231f074, gExtraBossHandBossAnimTable[0x67], 0);
    b->unk10C |= 4;
    b->unk110 |= 4;
    b->unk114 = 0x1000;
    ExtraBoss_SetCameraParams(0, 0x20000, -0x20000, 0, 0x2000);
    p->unkB8 &= ~0x200;
}
