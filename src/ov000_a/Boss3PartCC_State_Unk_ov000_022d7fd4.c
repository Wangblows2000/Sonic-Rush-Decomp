#include <nitro/types.h>

typedef struct UnkStruct_022d7fd4_ctl2 {
    u8 pad[4];
    u16 unk4;
} UnkStruct_022d7fd4_ctl2;

typedef struct UnkStruct_022d7fd4_ctl {
    s32 unk0;
    u8 pad[4];
    UnkStruct_022d7fd4_ctl2 *unk8;
} UnkStruct_022d7fd4_ctl;

typedef struct UnkStruct_022d7fd4_a {
    u8 pad[0x24];
    u8 unk24[0x24];
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    u8 padA[0xe4 - 0x54];
    struct UnkStruct_022d7fd4_ctl *unkE4;
    u8 padB[0x10c - 0xe8];
    u8 unk10C;
    u8 pad1[0x114 - 0x10d];
    s32 unk114;
    u8 pad2[0x170 - 0x118];
} UnkStruct_022d7fd4_a;

typedef struct UnkStruct_022d7fd4 UnkStruct_022d7fd4;
struct UnkStruct_022d7fd4 {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    u8 padA[0x2e - 0x18];
    s16 unk2E;
    s16 unk30;
    u8 pad0[0x5c - 0x32];
    u16 unk5C;
    u8 pad1[0x68 - 0x5e];
    void (*func)(UnkStruct_022d7fd4 *s);
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
    UnkStruct_022d7fd4_a unk2C4;
    void *unk434;
};

void Model_SetAnimation(void *p, s32 a, void *b, s32 c, s32 d);

typedef struct UnkStruct_022d7fd4_r {
    u8 pad[0x20c];
    u32 unk20C;
} UnkStruct_022d7fd4_r;

typedef struct UnkStruct_022d7fd4_b {
    u8 pad[0x20c];
    u32 unk20C;
    u8 pad1[0x218 - 0x210];
    UnkStruct_022d7fd4_r *unk218;
} UnkStruct_022d7fd4_b;

typedef struct UnkStruct_022d7fd4_c {
    u8 pad[0x30];
    u16 unk30;
    u8 pad1[0x38 - 0x32];
    s32 unk38;
    u8 pad2[0x58 - 0x3c];
    s32 unk58;
    u8 pad3[0x6c - 0x5c];
    u16 unk6C;
    u8 pad4[0x74 - 0x6e];
    s32 unk74;
    s32 unk78;
    s32 unk7C;
    u16 unk80;
    u8 pad5[0x84 - 0x82];
    s32 unk84;
    s32 unk88;
    u16 unk8C;
} UnkStruct_022d7fd4_c;

extern UnkStruct_022d7fd4_b *gBoss3;
extern UnkStruct_022d7fd4_c data_ov000_0236a48c;

void func_ov000_022daa78(void);
void func_ov000_022daa5c(void);
void func_ov000_022da6b4(void);
void BossArena_ArcToCirclePos(s32 len, s32 b, s32 a, s32 *outX, s32 *outY, u16 *outAng);
void BossEffect_CreateJump(s32 a, s32 b, s32 c, s32 d);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void Boss3PartCC_State_Unk_ov000_022d7f40(UnkStruct_022d7fd4 *s);

void Boss3PartCC_State_Unk_ov000_022d7fd4(UnkStruct_022d7fd4 *s) {
    UnkStruct_022d7fd4_r *r = gBoss3->unk218;
    UnkStruct_022d7fd4_c *c;
    Model_SetAnimation(&s->unk2C4, 0, s->unk434, 0xf, 0);
    s->unk2C4.unk10C &= ~4;
    s->unk2C4.unk114 = 0x1000;
    gBoss3->unk20C |= 2;
    func_ov000_022daa78();
    r->unk20C &= ~8;
    s->unk13E &= ~4;
    s->unk16A &= ~4;
    func_ov000_022daa5c();
    BossArena_ArcToCirclePos(0xf42f8, 0x5fe435, s->unk10 << 4, NULL, NULL, &s->unk258);
    c = (UnkStruct_022d7fd4_c *)(u32)&data_ov000_0236a48c;
    if (s->unk5C & 1)
        s->unk224 = s->unk258 - 0x4000;
    else
        s->unk224 = s->unk258 + 0x4000;
    s->unk258 += 0x8000;
    func_ov000_022da6b4();
    c->unk58 = -0x1a0000;
    c->unk6C = 0x400;
    c->unk74 = 0x4cc;
    c->unk30 = 0x80;
    c->unk38 = 0xccc;
    c->unk7C = 0x18a2f8;
    c->unk78 = c->unk7C;
    c->unk80 = 0x80;
    c->unk88 = -0x50000;
    c->unk84 = c->unk88;
    c->unk8C = 0x100;
    BossEffect_CreateJump(1, s->unk2C4.unk48 >> 4, -s->unk2C4.unk4C >> 4, s->unk2C4.unk50 >> 4);
    Sound_PlaySeqArc(0, 0, 0x91);
    s->func = Boss3PartCC_State_Unk_ov000_022d7f40;
}
