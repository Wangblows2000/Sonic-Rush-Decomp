#include <nitro/types.h>

typedef struct UnkStruct_022d6c0c_a {
    u8 pad[0x48];
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    u8 padA[0x10c - 0x54];
    u8 unk10C;
    u8 pad1[0x114 - 0x10d];
    s32 unk114;
    u8 pad2[0x170 - 0x118];
} UnkStruct_022d6c0c_a;

typedef struct UnkStruct_022d6c0c UnkStruct_022d6c0c;
struct UnkStruct_022d6c0c {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    u8 padA[0x2e - 0x18];
    s16 unk2E;
    s16 unk30;
    u8 pad0[0x5c - 0x32];
    u16 unk5C;
    u8 pad1[0x68 - 0x5e];
    void (*func)(UnkStruct_022d6c0c *s);
    u8 pad2[0x258 - 0x6c];
    u16 unk258;
    u16 unk25A;
    u8 pad3[0x264 - 0x25c];
    u16 unk264;
    u8 pad4[0x2c0 - 0x266];
    s32 unk2C0;
    UnkStruct_022d6c0c_a unk2C4;
    void *unk434;
};

void Model_SetAnimation(void *p, s32 a, void *b, s32 c, s32 d);

typedef struct UnkStruct_022d6c0c_r {
    u8 pad[0x20c];
    u32 unk20C;
} UnkStruct_022d6c0c_r;

typedef struct UnkStruct_022d6c0c_b {
    u8 pad[0x218];
    UnkStruct_022d6c0c_r *unk218;
} UnkStruct_022d6c0c_b;

extern UnkStruct_022d6c0c_b *gBoss3;

void BossEffect_CreateJump(s32 a, s32 b, s32 c, s32 d);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void Boss3PartCC_State_Unk_ov000_022d6b9c(UnkStruct_022d6c0c *s);

void Boss3PartCC_State_Unk_ov000_022d6c0c(UnkStruct_022d6c0c *s) {
    Model_SetAnimation(&s->unk2C4, 0, s->unk434, 6, 0);
    s->unk2C4.unk10C &= ~4;
    s->unk2C4.unk114 = 0x1000;
    gBoss3->unk218->unk20C &= ~0x40;
    BossEffect_CreateJump(1, s->unk2C4.unk48 >> 4, -s->unk2C4.unk4C >> 4, s->unk2C4.unk50 >> 4);
    Sound_PlaySeqArc(0, 0, 0x8f);
    s->func = Boss3PartCC_State_Unk_ov000_022d6b9c;
}
