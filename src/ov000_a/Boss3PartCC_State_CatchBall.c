#include <nitro/types.h>

typedef struct UnkStruct_022d66cc_a {
    u8 pad[0x10c];
    u8 unk10C;
    u8 pad1[0x114 - 0x10d];
    s32 unk114;
    u8 pad2[0x170 - 0x118];
} UnkStruct_022d66cc_a;

typedef struct UnkStruct_022d66cc UnkStruct_022d66cc;
struct UnkStruct_022d66cc {
    u8 pad[0x2e];
    s16 unk2E;
    u8 pad0[0x5c - 0x30];
    u16 unk5C;
    u8 pad1[0x68 - 0x5e];
    void (*func)(UnkStruct_022d66cc *s);
    u8 pad2[0x258 - 0x6c];
    u16 unk258;
    u16 unk25A;
    u8 pad3[0x264 - 0x25c];
    u16 unk264;
    u8 pad4[0x2c0 - 0x266];
    s32 unk2C0;
    UnkStruct_022d66cc_a unk2C4;
    void *unk434;
};

void Model_SetAnimation(void *p, s32 a, void *b, s32 c, s32 d);

typedef struct UnkStruct_022d66cc_r {
    u8 pad[0x2e];
    s16 unk2E;
    u8 pad1[0x20c - 0x30];
    u32 unk20C;
    u8 pad2[0x2d8 - 0x210];
    s32 unk2D8;
    s32 unk2DC;
    s32 unk2E0;
} UnkStruct_022d66cc_r;

typedef struct UnkStruct_022d66cc_b {
    u8 pad[0x20c];
    u32 unk20C;
    u8 pad1[0x218 - 0x210];
    UnkStruct_022d66cc_r *unk218;
} UnkStruct_022d66cc_b;

typedef struct UnkStruct_022d66cc_q {
    u8 pad[0x158];
    u8 unk158[0x24];
} UnkStruct_022d66cc_q;

typedef struct UnkStruct_022d66cc_p {
    u8 pad[0x32c];
    u32 unk32C;
} UnkStruct_022d66cc_p;

extern UnkStruct_022d66cc_b *gBoss3;
extern UnkStruct_022d66cc_p *gPlayers;
extern u32 gRandSeed;
extern s16 data_0206e470[];

UnkStruct_022d66cc_q *BossEffect_CreateCatch(s32 a, s32 b, s32 c, s32 d);
void MTX_RotX33_(void *m, s32 s, s32 c);
void MTX_RotY33_(void *m, s32 s, s32 c);
void MTX_Concat33(void *a, void *b, void *c);
void NNS_SndPlayerStopSeq(s32 a, s32 b);
void NNS_SndHandleReleaseSeq(s32 a);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void Boss3PartCC_State_CatchBallWait(UnkStruct_022d66cc *s);

static inline u16 Rand(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return (u16)(gRandSeed >> 16);
}

void Boss3PartCC_State_CatchBall(UnkStruct_022d66cc *s) {
    UnkStruct_022d66cc_r *r = gBoss3->unk218;
    UnkStruct_022d66cc_q *q;
    u8 mtx[0x24];
    u16 ang;
    Model_SetAnimation(&s->unk2C4, 0, s->unk434, 2, 0);
    s->unk2C4.unk10C &= ~4;
    s->unk2C4.unk114 = 0x1000;
    r->unk20C &= ~1;
    r->unk2E = 0;
    r->unk20C &= ~0x48;
    q = BossEffect_CreateCatch(1, r->unk2D8 >> 4, -r->unk2DC >> 4, r->unk2E0 >> 4);
    ang = (Rand() & 0xfff) + 0xaaa;
    MTX_RotX33_(q->unk158, data_0206e470[((ang & 0xffff) >> 4) * 2], data_0206e470[((ang & 0xffff) >> 4) * 2 + 1]);
    ang = Rand();
    MTX_RotY33_(mtx, data_0206e470[((ang & 0xffff) >> 4) * 2], data_0206e470[((ang & 0xffff) >> 4) * 2 + 1]);
    MTX_Concat33(q->unk158, mtx, q->unk158);
    gPlayers->unk32C &= ~0x200000;
    gBoss3->unk20C &= ~4;
    NNS_SndPlayerStopSeq(s->unk2C0, 0x20);
    NNS_SndHandleReleaseSeq(s->unk2C0);
    Sound_PlaySeqArc(0, 0, 0xcc);
    s->func = Boss3PartCC_State_CatchBallWait;
}
