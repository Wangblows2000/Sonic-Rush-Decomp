#include <nitro/types.h>

typedef struct UnkStruct_022d79a4_ctl2 {
    u8 pad[4];
    u16 unk4;
} UnkStruct_022d79a4_ctl2;

typedef struct UnkStruct_022d79a4_ctl {
    s32 unk0;
    u8 pad[4];
    UnkStruct_022d79a4_ctl2 *unk8;
} UnkStruct_022d79a4_ctl;

typedef struct UnkStruct_022d79a4_a {
    u8 pad[0x24];
    u8 unk24[0x24];
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    u8 padA[0xe4 - 0x54];
    struct UnkStruct_022d79a4_ctl *unkE4;
    u8 padB[0x10c - 0xe8];
    u8 unk10C;
    u8 pad1[0x114 - 0x10d];
    s32 unk114;
    u8 pad2[0x170 - 0x118];
} UnkStruct_022d79a4_a;

typedef struct UnkStruct_022d79a4 UnkStruct_022d79a4;
struct UnkStruct_022d79a4 {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    u8 padA[0x2e - 0x18];
    s16 unk2E;
    s16 unk30;
    u8 pad0[0x5c - 0x32];
    u16 unk5C;
    u8 pad1[0x68 - 0x5e];
    void (*func)(UnkStruct_022d79a4 *s);
    u8 pad2[0x13e - 0x6c];
    u16 unk13E;
    u8 pad2a[0x16a - 0x140];
    u16 unk16A;
    u8 pad2c[0x218 - 0x16c];
    s32 unk218;
    s32 unk21C;
    s32 unk220;
    u16 unk224;
    u8 pad2b[0x24c - 0x226];
    s32 unk24C;
    s32 unk250;
    s32 unk254;
    u16 unk258;
    u16 unk25A;
    u8 pad3[0x264 - 0x25c];
    u16 unk264;
    u8 pad4[0x2c0 - 0x266];
    s32 unk2C0;
    UnkStruct_022d79a4_a unk2C4;
    void *unk434;
};

void Model_SetAnimation(void *p, s32 a, void *b, s32 c, s32 d);

typedef struct UnkStruct_022d79a4_q {
    u8 pad[0x25e];
    u16 unk25E;
} UnkStruct_022d79a4_q;

typedef struct UnkStruct_022d79a4_r {
    u8 pad[0x20c];
    u32 unk20C;
} UnkStruct_022d79a4_r;

typedef struct UnkStruct_022d79a4_b {
    u8 pad[0x214];
    UnkStruct_022d79a4_q *unk214;
    UnkStruct_022d79a4_r *unk218;
    u8 pad1[0x222 - 0x21c];
    u16 unk222;
} UnkStruct_022d79a4_b;

typedef struct UnkStruct_022d79a4_p {
    u8 pad[0x2e];
    s16 unk2E;
    u8 pad1[0x34 - 0x30];
    s16 unk34;
    u8 pad2[0x58 - 0x36];
    u32 unk58;
    u8 pad3[0x17c - 0x5c];
    s32 unk17C;
    s32 unk180;
    s32 unk184;
} UnkStruct_022d79a4_p;

extern UnkStruct_022d79a4_b *gBoss3;
extern UnkStruct_022d79a4_p *gPlayers;
extern s32 data_ov000_0236327c[][5];
extern u32 gRandSeed;

u16 Boss_Atan2(s32 x, s32 z);
s32 FX_Inv(s32 a);
void func_ov000_022da9c0(u16 ang, s32 v);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void Boss3PartCC_State_Unk_ov000_022d7908(UnkStruct_022d79a4 *s);

static inline u16 Rand(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return (u16)(gRandSeed >> 16);
}

void func_ov000_022d79a4(UnkStruct_022d79a4 *s) {
    s32 v;
    u16 ang;
    s32 m;
    UnkStruct_022d79a4_p *p;
    Model_SetAnimation(&s->unk2C4, 0, s->unk434, 0x14, 0);
    s->unk2C4.unk10C &= ~4;
    s->unk2C4.unk114 = 0x1000;
    v = data_ov000_0236327c[gBoss3->unk222][gBoss3->unk214->unk25E];
    gBoss3->unk218->unk20C |= 3;
    ang = Boss_Atan2(gPlayers->unk17C, gPlayers->unk184);
    switch (gBoss3->unk222) {
    case 3:
    case 4:
    case 5:
        p = gPlayers;
        if (p->unk58 & 0x8000)
            m = p->unk2E;
        else
            m = p->unk34;
        {
            u16 k = FX_Inv(data_ov000_0236327c[gBoss3->unk222][gBoss3->unk214->unk25E]) * 15 >> 12;
            ang += (u16)(k * (m * 4));
        }
        break;
    case 6:
    case 7:
    case 8:
        ang = ang - 0x1fff + (Rand() & 0x3fff);
        break;
    }
    func_ov000_022da9c0(ang, v);
    gBoss3->unk218 = NULL;
    Sound_PlaySeqArc(0, 0, 0x97);
    s->func = Boss3PartCC_State_Unk_ov000_022d7908;
    s->func(s);
}
