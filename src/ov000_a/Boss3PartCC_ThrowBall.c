#include <nitro/types.h>

typedef struct UnkStruct_022da9c0_r {
    u8 pad[0x214];
    s32 unk214;
    u8 pad1[0x21c - 0x218];
    s32 unk21C;
    u8 pad2[0x222 - 0x220];
    u16 unk222;
} UnkStruct_022da9c0_r;

typedef struct UnkStruct_022da9c0_b {
    u8 pad[0x218];
    UnkStruct_022da9c0_r *unk218;
} UnkStruct_022da9c0_b;

extern UnkStruct_022da9c0_b *gBoss3;
extern s16 data_0206e470[];

void Boss3PartCD_Action_Throw(UnkStruct_022da9c0_r *r);

static inline s32 FX_Mul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

void Boss3PartCC_ThrowBall(u16 ang, s32 v) {
    UnkStruct_022da9c0_r *r = gBoss3->unk218;
    r->unk214 = FX_Mul(v, data_0206e470[((ang & 0xffff) >> 4) * 2]);
    r->unk21C = FX_Mul(v, data_0206e470[((ang & 0xffff) >> 4) * 2 + 1]);
    gBoss3->unk218->unk222 = ang;
    Boss3PartCD_Action_Throw(gBoss3->unk218);
}
