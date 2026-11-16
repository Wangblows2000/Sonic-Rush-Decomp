#include <nitro/types.h>

typedef struct UnkStruct_022d8a90_ctl2 {
    u8 pad[4];
    u16 unk4;
} UnkStruct_022d8a90_ctl2;

typedef struct UnkStruct_022d8a90_ctl {
    s32 unk0;
    u8 pad[4];
    UnkStruct_022d8a90_ctl2 *unk8;
} UnkStruct_022d8a90_ctl;

typedef struct UnkStruct_022d8a90_a {
    u8 pad[0x24];
    u8 unk24[0x24];
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    u8 padA[0xe4 - 0x54];
    struct UnkStruct_022d8a90_ctl *unkE4;
    u8 padB[0x10c - 0xe8];
    u8 unk10C;
    u8 pad1[0x114 - 0x10d];
    s32 unk114;
    u8 pad2[0x12e - 0x118];
    u16 unk12E;
    u8 pad3[0x170 - 0x130];
} UnkStruct_022d8a90_a;

typedef struct UnkStruct_022d8a90 UnkStruct_022d8a90;
struct UnkStruct_022d8a90 {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    u8 padA[0x2e - 0x18];
    s16 unk2E;
    s16 unk30;
    u8 pad0[0x5c - 0x32];
    u16 unk5C;
    u8 pad1[0x68 - 0x5e];
    void (*func)(UnkStruct_022d8a90 *s);
    u8 pad2[0x13e - 0x6c];
    u16 unk13E;
    u8 pad2a[0x16a - 0x140];
    u16 unk16A;
    u8 pad2c[0x20c - 0x16c];
    s32 unk20C;
    u8 pad2d[0x214 - 0x210];
    s32 unk214;
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
    u16 unk25C;
    u16 unk25E;
    u8 pad3[0x264 - 0x260];
    u16 unk264;
    u8 pad4[0x2c0 - 0x266];
    s32 unk2C0;
    UnkStruct_022d8a90_a unk2C4;
    void *unk434;
};

void Model_SetAnimation(void *p, s32 a, void *b, s32 c, s32 d);

typedef struct UnkStruct_022d8a90_r {
    u8 pad[0x5c];
    u16 unk5C;
    u8 pad1[0x20c - 0x5e];
    u32 unk20C;
    u8 pad2[0x212 - 0x210];
    s16 unk212;
} UnkStruct_022d8a90_r;

typedef struct UnkStruct_022d8a90_b {
    u8 pad[0x218];
    UnkStruct_022d8a90_r *unk218;
} UnkStruct_022d8a90_b;

extern UnkStruct_022d8a90_b *gBoss3;

UnkStruct_022d8a90_r *MapObject_Spawn(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i);
void func_ov000_022daa78(void);
void BossEffect_CreatePop(s32 a, s32 b, s32 c, s32 d);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void ScreenShake_Start(s32 a);
void Boss3PartCC_State_Unk_ov000_022d89a8(UnkStruct_022d8a90 *s);

void Boss3PartCC_State_Unk_ov000_022d8a90(UnkStruct_022d8a90 *s) {
    Model_SetAnimation(&s->unk2C4, 0, s->unk434, 3, 0);
    s->unk2C4.unk10C &= ~4;
    s->unk2C4.unk114 = 0x1000;
    gBoss3->unk218 = MapObject_Spawn(0xcd, 0, 0, 0, 0, 0, 0, 0, 0);
    gBoss3->unk218->unk5C &= ~0x20;
    gBoss3->unk218->unk212 = 0;
    gBoss3->unk218->unk20C |= 0x80;
    func_ov000_022daa78();
    BossEffect_CreatePop(1, s->unk24C >> 4, -s->unk250 >> 4, s->unk254 >> 4);
    Sound_PlaySeqArc(0, 0, 0x95);
    ScreenShake_Start(0xb);
    s->unk264 = 0x20;
    s->unk258 = 0;
    s->func = Boss3PartCC_State_Unk_ov000_022d89a8;
}
