#include <nitro/types.h>

typedef struct UnkStruct_022d8c64_ctl2 {
    u8 pad[4];
    u16 unk4;
} UnkStruct_022d8c64_ctl2;

typedef struct UnkStruct_022d8c64_ctl {
    s32 unk0;
    u8 pad[4];
    UnkStruct_022d8c64_ctl2 *unk8;
} UnkStruct_022d8c64_ctl;

typedef struct UnkStruct_022d8c64_a {
    u8 pad[0x24];
    u8 unk24[0x24];
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    u8 padA[0xe4 - 0x54];
    struct UnkStruct_022d8c64_ctl *unkE4;
    u8 padB[0x10c - 0xe8];
    u8 unk10C;
    u8 pad1[0x114 - 0x10d];
    s32 unk114;
    u8 pad2[0x12e - 0x118];
    u16 unk12E;
    u8 pad3[0x170 - 0x130];
} UnkStruct_022d8c64_a;

typedef struct UnkStruct_022d8c64 UnkStruct_022d8c64;
struct UnkStruct_022d8c64 {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    u8 padA[0x2e - 0x18];
    s16 unk2E;
    s16 unk30;
    u8 pad0[0x5c - 0x32];
    u16 unk5C;
    u8 pad1[0x68 - 0x5e];
    void (*func)(UnkStruct_022d8c64 *s);
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
    UnkStruct_022d8c64_a unk2C4;
    void *unk434;
};

void Model_SetAnimation(void *p, s32 a, void *b, s32 c, s32 d);

typedef struct UnkStruct_022d8c64_r {
    u8 pad[0x10];
    s32 unk10;
    u8 pad1[0x2e - 0x14];
    s16 unk2E;
} UnkStruct_022d8c64_r;

typedef struct UnkStruct_022d8c64_b {
    u8 pad[0x218];
    UnkStruct_022d8c64_r *unk218;
} UnkStruct_022d8c64_b;

typedef struct UnkStruct_022d8c64_c {
    UnkStruct_022d8c64_r *unk0;
    u8 pad[4];
    UnkStruct_022d8c64_r *unk8;
} UnkStruct_022d8c64_c;

typedef struct UnkStruct_022d8c64_d {
    u8 pad[0xdc];
    s32 unkDC;
} UnkStruct_022d8c64_d;

extern UnkStruct_022d8c64_b *gBoss3;
extern UnkStruct_022d8c64_c data_ov000_0236a48c;
extern UnkStruct_022d8c64_d data_ov000_0236a444;

s32 Boss3_Unk_ov000_022da7c8(s32 a, s32 b, s32 c);
void BossArena_CirclePosToArc(s32 a, s32 b, s32 x, s32 z, s32 *out);

BOOL Boss3PartCC_Unk_ov000_022d8c64(UnkStruct_022d8c64 *s) {
    s32 v;
    if (data_ov000_0236a444.unkDC == 1) {
        UnkStruct_022d8c64_c *c = (UnkStruct_022d8c64_c *)(u32)&data_ov000_0236a48c;
        UnkStruct_022d8c64_r *r = gBoss3->unk218;
        if (Boss3_Unk_ov000_022da7c8(r->unk10, s->unk10, r->unk2E <= 0) < 0xc800) {
            c->unk0 = NULL;
            c->unk8 = NULL;
        } else {
            c->unk0 = gBoss3->unk218;
            c->unk8 = gBoss3->unk218;
        }
    }
    BossArena_CirclePosToArc(0xf42f8, 0x5fe435, s->unk24C, s->unk254, &v);
    v >>= 4;
    if (Boss3_Unk_ov000_022da7c8(v, gBoss3->unk218->unk10, !(s->unk5C & 1)) < 0x800) {
        if (s->unk5C & 1) {
            if (gBoss3->unk218->unk2E < 0)
                return TRUE;
        } else if (gBoss3->unk218->unk2E > 0) {
            return TRUE;
        }
    }
    return FALSE;
}
