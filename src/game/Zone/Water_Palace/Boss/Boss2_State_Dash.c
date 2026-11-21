#include <nitro/types.h>

typedef struct UnkStruct_022d3568_a {
    u8 pad[0xb];
    u8 unkB;
    u8 pad0[0x24 - 0xc];
    s32 unk24[9];
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    u8 pad1[0x90 - 0x54];
    u8 unk90[0xe4 - 0x90];
    s32 *unkE4;
    u8 padE8[0x10c - 0xe8];
    u8 unk10C;
    u8 pad2[0x114 - 0x10d];
    s32 unk114;
    u8 pad3[0x12c - 0x118];
    s16 unk12C;
    u8 pad4[0x138 - 0x12e];
} UnkStruct_022d3568_a;

typedef struct UnkStruct_022d3568_h {
    u8 pad[0xa];
    u16 unkA;
    u8 pad1[0x2c - 0xc];
} UnkStruct_022d3568_h;

typedef struct UnkStruct_022d3568_q {
    u8 pad[8];
    u16 unk8;
    u8 unkA;
    u8 unkB;
} UnkStruct_022d3568_q;

typedef struct UnkStruct_022d3568_o {
    u8 pad[0x54];
    u32 unk54;
    u8 pad0[0x5c - 0x58];
    u16 unk5C;
    u8 pad1[0x68 - 0x5e];
    void *unk68;
    u8 pad2[0x134 - 0x6c];
    UnkStruct_022d3568_q unk134;
} UnkStruct_022d3568_o;

typedef struct UnkStruct_022d3568 UnkStruct_022d3568;
struct UnkStruct_022d3568 {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    u8 pad0[0x5c - 0x18];
    u16 unk5C;
    u8 pad0b[0x68 - 0x5e];
    void (*func)(UnkStruct_022d3568 *s);
    u8 pad1[0x20c - 0x6c];
    u32 unk20C;
    u8 pad2[0xa9c - 0x210];
    UnkStruct_022d3568_a unkA9C;
    UnkStruct_022d3568_a unkBD4;
    UnkStruct_022d3568_a unkD0C;
    u8 pad3[0xf7c - 0xe44];
    s32 unkF7C;
    u8 pad3b[0xf84 - 0xf80];
    UnkStruct_022d3568_h unkF84[10];
    u8 pad4[0x12f4 - 0x113c];
    s32 unk12F4;
    u8 pad4b[0x1304 - 0x12f8];
    u16 unk1304;
    u8 pad5[0x131c - 0x1306];
    UnkStruct_022d3568_o *unk131C;
    u8 pad5b[0x139c - 0x1320];
    s32 unk139C;
    s32 unk13A0;
    s32 unk13A4;
    u8 pad5c[0x1450 - 0x13a8];
    s32 unk1450;
    u8 pad5d[0x1458 - 0x1454];
    s32 unk1458;
    s32 unk145C;
    s32 unk1460;
    s32 unk1464;
    s32 unk1468;
    s16 unk146C;
    u8 pad7[0x1470 - 0x146e];
    s32 unk1470;
    s32 unk1474;
};

void Model_SetAnimation(void *p, s32 a, s32 b, s32 c, s32 d);
void Boss2_SetPalettes(u32 flags);

typedef struct UnkStruct_022d3568_p {
    u8 pad[0x10];
    s32 unk10;
    u8 pad0[0x2e - 0x14];
    s16 unk2E;
    u8 pad1[0x34 - 0x30];
    s16 unk34;
    u8 pad2[0x58 - 0x36];
    u32 unk58;
} UnkStruct_022d3568_p;

typedef struct UnkStruct_022d3568_7 {
    u8 pad[8];
    u16 unk8;
    u16 unkA;
} UnkStruct_022d3568_7;

extern UnkStruct_022d3568_p *gPlayers;
extern UnkStruct_022d3568_7 gBossStatus;

s32 FX_DivS32(s32 a, s32 b);
void NNS_SndPlayerStopSeq(s32 a, s32 b);
void Boss2_UpdateArenaSpeed(UnkStruct_022d3568 *s, s32 mode);
void Boss2_State_DashHop(UnkStruct_022d3568 *s);
void Boss2_State_JumpRise(UnkStruct_022d3568 *s);

static inline s32 Lerp(s32 a, s32 b) {
    return FX_DivS32(gBossStatus.unk8 * a + (gBossStatus.unkA - gBossStatus.unk8) * b, gBossStatus.unkA);
}

static inline s32 GetDist(UnkStruct_022d3568 *s) {
    s32 d = gPlayers->unk10;
    s32 a;
    s32 b;
    if (d < s->unk10) {
        a = s->unk10 - d;
        b = d + 0x6cd98 - s->unk10;
    } else {
        a = s->unk10 + 0x6cd98 - d;
        b = d - s->unk10;
    }
    if (s->unk1460 < 0)
        return a;
    return b;
}

void Boss2_State_Dash(UnkStruct_022d3568 *s) {
    UnkStruct_022d3568_a *x = &s->unkA9C;
    UnkStruct_022d3568_a *y = &s->unkBD4;
    UnkStruct_022d3568_a *z = &s->unkD0C;
    UnkStruct_022d3568_p *p;
    s32 a;
    s32 v;
    s32 m;
    s32 t;
    Boss2_UpdateArenaSpeed(s, 0);
    s->unk12F4--;
    if (s->unk12F4 <= 0) {
        s->func = Boss2_State_DashHop;
        s->unk146C = 0;
        NNS_SndPlayerStopSeq(s->unk1450, 0);
        return;
    }
    if (s->unk1470 == 0 && (x->unk10C & 2)) {
        if (s->unk1460 < 0) {
            Model_SetAnimation(x, 0, s->unkF7C, 6, 0);
            Model_SetAnimation(y, 0, s->unkF7C, 0x26, 0);
            Model_SetAnimation(z, 0, s->unkF7C, 0x42, 0);
        } else {
            Model_SetAnimation(x, 0, s->unkF7C, 9, 0);
            Model_SetAnimation(y, 0, s->unkF7C, 0x29, 0);
            Model_SetAnimation(z, 0, s->unkF7C, 0x45, 0);
        }
        x->unk10C |= 4;
        y->unk10C |= 4;
        z->unk10C |= 4;
        x->unk114 = 0x1000;
        y->unk114 = 0x1000;
        z->unk114 = 0x1000;
        s->unk1470 = 1;
        return;
    }
    if (s->unk1470 == 0)
        return;
    p = gPlayers;
    if (p->unk58 & 0x8000)
        v = p->unk2E;
    else
        v = p->unk34;
    s->unk1464 = s->unk1474;
    if (v >= 0)
    {
        t = (v < 0x47a) ? 0 : v - 0x47a;
        s->unk1464 += t;
    } else {
        t = (v > -0x47a) ? 0 : v + 0x47a;
        s->unk1464 += t;
    }
    if ((s->unk1460 > 0 && v > 0) || (s->unk1460 < 0 && v < 0)) {
        a = GetDist(s);
        if (s->unk1460 > 0)
            m = v * Lerp(-0x28, -10);
        else
            m = v * Lerp(0x28, 10);
        m += ((s->unk145C < 0) ? -s->unk145C : s->unk145C) * 10 + 0x19000;
        if (m < 0)
            m = 0;
        if (a > m)
            return;
        s->func = Boss2_State_JumpRise;
        x = &s->unkA9C;
        x->unk10C &= ~4;
        y = &s->unkBD4;
        y->unk10C &= ~4;
        z = &s->unkD0C;
        z->unk10C &= ~4;
        z->unk114 = 0x2000;
        y->unk114 = z->unk114;
        x->unk114 = y->unk114;
        s->unk1470 = 0;
        s->unk146C = 0;
        if (s->unk1460 < 0)
            s->unk1474 = -Lerp(0x180, 0x2a0);
        else
            s->unk1474 = Lerp(0x180, 0x2a0);
        s->unk1468 = s->unk1474 + v;
        s->unk1460 <<= 2;
    }
}
