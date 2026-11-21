#include <nitro/types.h>

typedef struct UnkStruct_022d0e9c_a {
    u8 pad[0xb];
    u8 unkB;
    u8 pad0[0x24 - 0xc];
    s32 unk24[9];
    u8 pad1[0x90 - 0x48];
    u8 unk90[0xe4 - 0x90];
    s32 *unkE4;
    u8 padE8[0x10c - 0xe8];
    u8 unk10C;
    u8 pad2[0x114 - 0x10d];
    s32 unk114;
    u8 pad3[0x138 - 0x118];
} UnkStruct_022d0e9c_a;

typedef struct UnkStruct_022d0e9c_h {
    u8 pad[0xa];
    u16 unkA;
    u8 pad1[0x2c - 0xc];
} UnkStruct_022d0e9c_h;

typedef struct UnkStruct_022d0e9c UnkStruct_022d0e9c;
struct UnkStruct_022d0e9c {
    u8 pad[0x10];
    s32 unk10;
    u8 pad0[0x68 - 0x14];
    void (*func)(UnkStruct_022d0e9c *s);
    u8 pad1[0x20c - 0x6c];
    u32 unk20C;
    u8 pad2[0xa9c - 0x210];
    UnkStruct_022d0e9c_a unkA9C;
    UnkStruct_022d0e9c_a unkBD4;
    UnkStruct_022d0e9c_a unkD0C;
    u8 pad3[0xf7c - 0xe44];
    s32 unkF7C;
    u8 pad3b[0xf84 - 0xf80];
    UnkStruct_022d0e9c_h unkF84[10];
    u8 pad4[0x12f4 - 0x113c];
    s32 unk12F4;
    u8 pad4b[0x1304 - 0x12f8];
    u16 unk1304;
    u8 pad5[0x131c - 0x1306];
    u8 *unk131C;
    u8 pad5b[0x139c - 0x1320];
    s32 unk139C;
    s32 unk13A0;
    s32 unk13A4;
    u8 pad5c[0x1458 - 0x13a8];
    s32 unk1458;
    u16 unk145C;
    u8 pad6[0x1460 - 0x145e];
    s32 unk1460;
    s32 unk1464;
    u8 pad7[0x1474 - 0x1468];
    s32 unk1474;
};

void Model_SetAnimation(void *p, s32 a, s32 b, s32 c, s32 d);
void Boss2_SetPalettes(u32 flags);

typedef struct UnkStruct_022d0e9c_7 {
    u8 pad[8];
    u16 unk8;
    u16 unkA;
} UnkStruct_022d0e9c_7;

typedef struct UnkStruct_022d0e9c_pl {
    u8 pad[0x10];
    s32 unk10;
} UnkStruct_022d0e9c_pl;

extern UnkStruct_022d0e9c_7 gBossStatus;
extern UnkStruct_022d0e9c_pl *gPlayers;

s32 FX_DivS32(s32 a, s32 b);
void NNS_SndPlayerStopSeq(s32 a, s32 b);
void Sound_PlayVoice(s32 a, s32 b, s32 c);
void BossArena_CirclePosToArc(s32 unused, s32 len, s32 x, s32 y, s32 *out);
void Boss2_State_Attack3Catch(UnkStruct_022d0e9c *s);
void Boss2_State_Attack3Hold(UnkStruct_022d0e9c *s);

void Boss2_State_Attack3Suck(UnkStruct_022d0e9c *s) {
    UnkStruct_022d0e9c_a *x = &s->unkA9C;
    s32 p;
    s32 a;
    s32 d1;
    UnkStruct_022d0e9c_a *y = &s->unkBD4;
    UnkStruct_022d0e9c_a *z = &s->unkD0C;
    s32 d2;
    if (y->unk10C & 2) {
        s->func = Boss2_State_Attack3Catch;
        if (s->unk145C) {
            Model_SetAnimation(x, 0, s->unkF7C, 0x19, 0);
            Model_SetAnimation(z, 0, s->unkF7C, 0x55, 0);
        } else {
            Model_SetAnimation(x, 0, s->unkF7C, 0x13, 0);
            Model_SetAnimation(z, 0, s->unkF7C, 0x4f, 0);
        }
        x->unk10C &= ~4;
        z->unk10C &= ~4;
        x->unk114 = 0x1000;
        z->unk114 = 0x1000;
        NNS_SndPlayerStopSeq(s->unk1458, 0);
        Sound_PlayVoice(0, 0, 0xd2);
        *(u32 *)(s->unk131C + 0x54) |= 4;
        return;
    }
    BossArena_CirclePosToArc(0x1152f0, 0x6cd988, s->unk139C, s->unk13A4, &a);
    p = gPlayers->unk10 << 4;
    if (p < a) {
        d1 = a - p;
        d2 = p + 0x6cd988 - a;
    } else {
        d2 = p - a;
        d1 = a + 0x6cd988 - p;
    }
    if (s->unk145C == 0)
        d2 = d1;
    if (d2 > FX_DivS32(gBossStatus.unk8 * 0xfa000 + (gBossStatus.unkA - gBossStatus.unk8) * 0xb4000, gBossStatus.unkA))
        return;
    s->func = Boss2_State_Attack3Hold;
    y->unk114 = 0;
    s->unk1464 = d2;
    s->unk12F4 = FX_DivS32(gBossStatus.unk8 * 0x14 + (gBossStatus.unkA - gBossStatus.unk8) * 0xa, gBossStatus.unkA);
    NNS_SndPlayerStopSeq(s->unk1458, 0);
}
