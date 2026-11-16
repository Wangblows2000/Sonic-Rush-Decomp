#include <nitro/types.h>

typedef struct UnkStruct_022d32c0_a {
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
} UnkStruct_022d32c0_a;

typedef struct UnkStruct_022d32c0_h {
    u8 pad[0xa];
    u16 unkA;
    u8 pad1[0x2c - 0xc];
} UnkStruct_022d32c0_h;

typedef struct UnkStruct_022d32c0_q {
    u8 pad[8];
    u16 unk8;
    u8 unkA;
    u8 unkB;
} UnkStruct_022d32c0_q;

typedef struct UnkStruct_022d32c0_o {
    u8 pad[0x54];
    u32 unk54;
    u8 pad0[0x5c - 0x58];
    u16 unk5C;
    u8 pad1[0x68 - 0x5e];
    void *unk68;
    u8 pad2[0x134 - 0x6c];
    UnkStruct_022d32c0_q unk134;
} UnkStruct_022d32c0_o;

typedef struct UnkStruct_022d32c0 UnkStruct_022d32c0;
struct UnkStruct_022d32c0 {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    u8 pad0[0x5c - 0x18];
    u16 unk5C;
    u8 pad0b[0x68 - 0x5e];
    void (*func)(UnkStruct_022d32c0 *s);
    u8 pad1[0x20c - 0x6c];
    u32 unk20C;
    u8 pad2[0xa9c - 0x210];
    UnkStruct_022d32c0_a unkA9C;
    UnkStruct_022d32c0_a unkBD4;
    UnkStruct_022d32c0_a unkD0C;
    u8 pad3[0xf7c - 0xe44];
    s32 unkF7C;
    u8 pad3b[0xf84 - 0xf80];
    UnkStruct_022d32c0_h unkF84[10];
    u8 pad4[0x12f4 - 0x113c];
    s32 unk12F4;
    u8 pad4b[0x1304 - 0x12f8];
    u16 unk1304;
    u8 pad5[0x131c - 0x1306];
    UnkStruct_022d32c0_o *unk131C;
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
void func_ov000_022cd1b0(u32 flags);

typedef struct UnkStruct_022d32c0_p {
    u8 pad[0x2e];
    s16 unk2E;
    u8 pad1[0x34 - 0x30];
    s16 unk34;
    u8 pad2[0x58 - 0x36];
    u32 unk58;
} UnkStruct_022d32c0_p;

typedef struct UnkStruct_022d32c0_7 {
    u8 pad[8];
    u16 unk8;
    u16 unkA;
} UnkStruct_022d32c0_7;

extern UnkStruct_022d32c0_p *gPlayers;
extern UnkStruct_022d32c0_7 data_ov000_02369ac0;

s32 FX_DivS32(s32 a, s32 b);
void NNS_SndPlayerStopSeq(s32 a, s32 b);
void func_020366f4(s32 a, s32 b, s32 c);
void func_ov000_022d2920(UnkStruct_022d32c0 *s, s32 mode);
void func_ov000_022d31c4(UnkStruct_022d32c0 *s);

static inline s32 FX_Mul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

static inline s32 Ease(s32 t) {
    s32 v;
    s32 n;
    v = 0;
    n = 1;
    do {
        v = FX_Mul(v - 0x2000, t) + 0x2000;
    } while (n-- != 0);
    return v;
}

static inline s32 Lerp(s32 a, s32 b) {
    return FX_DivS32(data_ov000_02369ac0.unk8 * a + (data_ov000_02369ac0.unkA - data_ov000_02369ac0.unk8) * b, data_ov000_02369ac0.unkA);
}

void func_ov000_022d32c0(UnkStruct_022d32c0 *s) {
    UnkStruct_022d32c0_p *p = gPlayers;
    UnkStruct_022d32c0_a *x = &s->unkA9C;
    UnkStruct_022d32c0_a *y = &s->unkBD4;
    UnkStruct_022d32c0_a *z = &s->unkD0C;
    s32 v;
    s32 t;
    if (p->unk58 & 0x8000)
        v = p->unk2E;
    else
        v = p->unk34;
    t = s->unk1474 + v;
    if (s->unk1474 > 0) {
        if (t < 0)
            t = 0;
    } else {
        if (t > 0)
            t = 0;
    }
    s->unk1468 = t;
    func_ov000_022d2920(s, 1);
    s->unk146C += 0xcc;
    s->unk146C = (s->unk146C >= 0x1000) ? 0x1000 : s->unk146C;
    s->unk14 = Ease(s->unk146C) + 0x20000;
    if (!(x->unk10C & 2))
        return;
    if (s->unk1470 == 0) {
        if (s->unk1460 < 0) {
            Model_SetAnimation(x, 0, s->unkF7C, 7, 0);
            Model_SetAnimation(y, 0, s->unkF7C, 0x27, 0);
            Model_SetAnimation(z, 0, s->unkF7C, 0x43, 0);
        } else {
            Model_SetAnimation(x, 0, s->unkF7C, 0xa, 0);
            Model_SetAnimation(y, 0, s->unkF7C, 0x2a, 0);
            Model_SetAnimation(z, 0, s->unkF7C, 0x46, 0);
        }
        s->unk1470 = 1;
    } else {
        s->func = func_ov000_022d31c4;
        s->unk14 = 0x20000;
        Model_SetAnimation(x, 0, s->unkF7C, 0x1d, 0);
        Model_SetAnimation(y, 0, s->unkF7C, 0x39, 0);
        Model_SetAnimation(z, 0, s->unkF7C, 0x59, 0);
        z->unk114 = Lerp(0xccc, 0x1333);
        y->unk114 = z->unk114;
        x->unk114 = y->unk114;
        NNS_SndPlayerStopSeq(s->unk1450, 0);
        func_020366f4(0, 0, 0xd0);
    }
}
