#include <nitro/types.h>

typedef struct UnkStruct_022d7028_ctl2 {
    u8 pad[4];
    u16 unk4;
} UnkStruct_022d7028_ctl2;

typedef struct UnkStruct_022d7028_ctl {
    s32 unk0;
    u8 pad[4];
    UnkStruct_022d7028_ctl2 *unk8;
} UnkStruct_022d7028_ctl;

typedef struct UnkStruct_022d7028_a {
    u8 pad[0x48];
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    u8 padA[0xe4 - 0x54];
    struct UnkStruct_022d7028_ctl *unkE4;
    u8 padB[0x10c - 0xe8];
    u8 unk10C;
    u8 pad1[0x114 - 0x10d];
    s32 unk114;
    u8 pad2[0x170 - 0x118];
} UnkStruct_022d7028_a;

typedef struct UnkStruct_022d7028 UnkStruct_022d7028;
struct UnkStruct_022d7028 {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    u8 padA[0x2e - 0x18];
    s16 unk2E;
    s16 unk30;
    u8 pad0[0x5c - 0x32];
    u16 unk5C;
    u8 pad1[0x68 - 0x5e];
    void (*func)(UnkStruct_022d7028 *s);
    u8 pad2[0x258 - 0x6c];
    u16 unk258;
    u16 unk25A;
    u8 pad3[0x264 - 0x25c];
    u16 unk264;
    u8 pad4[0x2c0 - 0x266];
    s32 unk2C0;
    UnkStruct_022d7028_a unk2C4;
    void *unk434;
};

void Model_SetAnimation(void *p, s32 a, void *b, s32 c, s32 d);

typedef struct UnkStruct_022d7028_r {
    u8 pad[0x2e];
    s16 unk2E;
    u8 pad1[0x20c - 0x30];
    u32 unk20C;
    u8 pad2[0x224 - 0x210];
    u16 unk224;
} UnkStruct_022d7028_r;

typedef struct UnkStruct_022d7028_b {
    u8 pad[0x218];
    UnkStruct_022d7028_r *unk218;
    u8 pad1[0x222 - 0x21c];
    u16 unk222;
} UnkStruct_022d7028_b;

typedef struct UnkStruct_022d7028_4 {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
} UnkStruct_022d7028_4;

extern UnkStruct_022d7028_b *gBoss3;
extern UnkStruct_022d7028_4 gGameState;
extern s16 data_ov000_023630b8[];
extern u16 data_ov000_02363130[][6];

void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void Boss3PartCC_State_KickFollowThrough(UnkStruct_022d7028 *s);

static inline s32 GetIdx(void) {
    BOOL b = TRUE;
    if (gGameState.unk10 != 2 && gGameState.unk10 != 3)
        b = FALSE;
    if (b)
        return 1;
    return gGameState.unk14;
}

void Boss3PartCC_State_KickBall(UnkStruct_022d7028 *s) {
    UnkStruct_022d7028_r *r = gBoss3->unk218;
    Model_SetAnimation(&s->unk2C4, 0, s->unk434, 0xe, 0);
    s->unk2C4.unk10C &= ~4;
    s->unk2C4.unk114 = 0x1000;
    r->unk20C |= 1;
    if (s->unk5C & 1)
        r->unk2E = data_ov000_023630b8[gBoss3->unk222];
    else
        r->unk2E = -data_ov000_023630b8[gBoss3->unk222];
    r->unk20C |= 8;
    r->unk224 = data_ov000_02363130[GetIdx()][gBoss3->unk222];
    Sound_PlaySeqArc(s->unk2C0, 0, 0x93);
    s->func = Boss3PartCC_State_KickFollowThrough;
}
