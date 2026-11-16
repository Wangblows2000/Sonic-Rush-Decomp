#include <nitro/types.h>

typedef struct UnkStruct_022cdbb0_a {
    u8 pad[0xe4];
    s32 *unkE4;
    u8 pad1[0x10c - 0xe8];
    u8 unk10C;
    u8 pad2[0x114 - 0x10d];
    s32 unk114;
    u8 pad3[0x138 - 0x118];
} UnkStruct_022cdbb0_a;

typedef struct UnkStruct_022cdbb0_h {
    u8 pad[0xa];
    u16 unkA;
    u8 pad1[0x2c - 0xc];
} UnkStruct_022cdbb0_h;

typedef struct UnkStruct_022cdbb0 UnkStruct_022cdbb0;
struct UnkStruct_022cdbb0 {
    u8 pad[0x68];
    void (*func)(UnkStruct_022cdbb0 *s);
    u8 pad1[0xa9c - 0x6c];
    UnkStruct_022cdbb0_a unkA9C;
    UnkStruct_022cdbb0_a unkBD4;
    UnkStruct_022cdbb0_a unkD0C;
    u8 pad2[0xf7c - 0xe44];
    s32 unkF7C;
    u8 pad3[0xfb0 - 0xf80];
    UnkStruct_022cdbb0_h unkFB0[9];
    u8 pad7[0x139c - 0x113c];
    s32 unk139C;
    s32 unk13A0;
    s32 unk13A4;
    u8 pad8[0x1458 - 0x13a8];
    s32 unk1458;
    u16 unk145C;
    u8 pad10[0x1474 - 0x145e];
    s32 unk1474;
};

typedef struct UnkStruct_022cdbb0_c {
    u8 pad[0x18];
    void *unk18;
} UnkStruct_022cdbb0_c;

typedef struct UnkStruct_022cdbb0_2 {
    u8 unk0;
} UnkStruct_022cdbb0_2;

typedef struct UnkStruct_022cdbb0_7 {
    u8 pad[4];
    s32 unk4;
    u16 unk8;
} UnkStruct_022cdbb0_7;

extern UnkStruct_022cdbb0_7 gBossStatus;
extern u32 gRandSeed;
extern s32 data_ov000_02362e14[];

void Boss2_Action_Defeat(void);
void Boss2_State_Unk_ov000_022d14e8(UnkStruct_022cdbb0 *s);
void Model_SetAnimation(void *p, s32 a, s32 b, s32 c, s32 d);
void func_ov000_022cd1b0(u32 flags);
void func_ov000_022cedd8(UnkStruct_022cdbb0 *s);
void BossEffect_CreateHit(s32 a, s32 x, s32 y, s32 z);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void Player_Action_AttackBounce(UnkStruct_022cdbb0_2 *p);
void func_020537b4(s32 a);
void func_020533e0(s32 a, s32 b);
void Sound_PlaySeq(s32 a, s32 b);
void Sound_PlayVoice(s32 a, s32 b, s32 c);

static inline u16 Rand(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return (u16)(gRandSeed >> 16);
}

void Boss2_OnHit_Unk_ov000_022cdbb0(UnkStruct_022cdbb0_c *a, UnkStruct_022cdbb0_c *b) {
    UnkStruct_022cdbb0 *s = a->unk18;
    UnkStruct_022cdbb0_a *x = &s->unkA9C;
    UnkStruct_022cdbb0_a *y = &s->unkBD4;
    UnkStruct_022cdbb0_a *z = &s->unkD0C;
    UnkStruct_022cdbb0_2 *p;
    if (gBossStatus.unk8 != 0) {
    gBossStatus.unk8--;
    s->unkFB0[0].unkA &= ~4;
    s->unkFB0[0].unkA |= 0x1000;
    s->unkFB0[3].unkA &= ~4;
    s->unkFB0[3].unkA |= 0x1000;
    s->unkFB0[6].unkA &= ~4;
    s->unkFB0[6].unkA |= 0x1000;
    s->unkFB0[1].unkA &= ~4;
    s->unkFB0[1].unkA |= 0x1000;
    if (gBossStatus.unk8 != 0) {
    s->func = Boss2_State_Unk_ov000_022d14e8;
    Model_SetAnimation(x, 0, s->unkF7C, 0x10, 0);
    Model_SetAnimation(y, 0, s->unkF7C, 0x30, 0);
    Model_SetAnimation(z, 0, s->unkF7C, 0x4c, 0);
    x->unk10C &= ~4;
    y->unk10C &= ~4;
    z->unk10C &= ~4;
    x->unk114 = 0x1000;
    y->unk114 = 0x1000;
    z->unk114 = 0x1000;
    func_ov000_022cd1b0(0);
    s->unk1474 = 0;
    gBossStatus.unk4 = 0xb;
    BossEffect_CreateHit(0, s->unk139C >> 4, (-s->unk13A0 >> 4) - 0x3200, s->unk13A4 >> 4);
    p = b->unk18;
    if (p != NULL && p->unk0 == 1)
        Player_Action_AttackBounce(p);
    if (gBossStatus.unk8 == 2) {
        func_020537b4(0xb);
        func_020533e0(0x12, 0xf0);
        Sound_PlaySeq(0, 0xf);
    } else {
        func_020537b4(0xb);
        func_020533e0(0x10, 0xf0);
    }
    Sound_PlaySeqArc(0, 0, 0x7b);
    Sound_PlayVoice(0, 0, data_ov000_02362e14[Rand() & 1]);
    } else {
        Boss2_Action_Defeat();
    }
    } else {
        Boss2_Action_Defeat();
    }
}
