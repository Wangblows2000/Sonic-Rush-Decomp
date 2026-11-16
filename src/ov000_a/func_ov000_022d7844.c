#include <nitro/types.h>

typedef struct UnkStruct_022d7844_ctl2 {
    u8 pad[4];
    u16 unk4;
} UnkStruct_022d7844_ctl2;

typedef struct UnkStruct_022d7844_ctl {
    s32 unk0;
    u8 pad[4];
    UnkStruct_022d7844_ctl2 *unk8;
} UnkStruct_022d7844_ctl;

typedef struct UnkStruct_022d7844_a {
    u8 pad[0x24];
    u8 unk24[0x24];
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    u8 padA[0xe4 - 0x54];
    struct UnkStruct_022d7844_ctl *unkE4;
    u8 padB[0x10c - 0xe8];
    u8 unk10C;
    u8 pad1[0x114 - 0x10d];
    s32 unk114;
    u8 pad2[0x170 - 0x118];
} UnkStruct_022d7844_a;

typedef struct UnkStruct_022d7844 UnkStruct_022d7844;
struct UnkStruct_022d7844 {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    u8 padA[0x2e - 0x18];
    s16 unk2E;
    s16 unk30;
    u8 pad0[0x5c - 0x32];
    u16 unk5C;
    u8 pad1[0x68 - 0x5e];
    void (*func)(UnkStruct_022d7844 *s);
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
    UnkStruct_022d7844_a unk2C4;
    void *unk434;
};

void Model_SetAnimation(void *p, s32 a, void *b, s32 c, s32 d);

typedef struct UnkStruct_022d7844_r {
    u8 pad[0x5c];
    u16 unk5C;
} UnkStruct_022d7844_r;

typedef struct UnkStruct_022d7844_b {
    u8 pad[0x218];
    UnkStruct_022d7844_r *unk218;
} UnkStruct_022d7844_b;

extern UnkStruct_022d7844_b *data_ov000_02369b14;

UnkStruct_022d7844_r *MapObject_Spawn(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i);
void func_ov000_022daa5c(void);
void BossEffect_CreatePop(s32 a, s32 b, s32 c, s32 d);
void func_ov000_022d77d4(UnkStruct_022d7844 *s);

void func_ov000_022d7844(UnkStruct_022d7844 *s) {
    Model_SetAnimation(&s->unk2C4, 0, s->unk434, 4, 0);
    s->unk2C4.unk10C &= ~4;
    s->unk2C4.unk114 = 0x1000;
    data_ov000_02369b14->unk218 = MapObject_Spawn(0xcd, 0, 0, 0, 0, 0, 0, 0, 0);
    data_ov000_02369b14->unk218->unk5C &= ~0x20;
    func_ov000_022daa5c();
    BossEffect_CreatePop(1, s->unk24C >> 4, -s->unk250 >> 4, s->unk254 >> 4);
    s->func = func_ov000_022d77d4;
}
