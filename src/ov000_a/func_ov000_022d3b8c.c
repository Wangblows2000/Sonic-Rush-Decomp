#include <nitro/types.h>

typedef struct UnkStruct_022d3b8c_a {
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
} UnkStruct_022d3b8c_a;

typedef struct UnkStruct_022d3b8c_h {
    u8 pad[0xa];
    u16 unkA;
    u8 pad1[0x2c - 0xc];
} UnkStruct_022d3b8c_h;

typedef struct UnkStruct_022d3b8c_q {
    u8 pad[8];
    u16 unk8;
    u8 unkA;
    u8 unkB;
} UnkStruct_022d3b8c_q;

typedef struct UnkStruct_022d3b8c_o {
    u8 pad[0x54];
    u32 unk54;
    u8 pad0[0x5c - 0x58];
    u16 unk5C;
    u8 pad1[0x68 - 0x5e];
    void *unk68;
    u8 pad2[0x134 - 0x6c];
    UnkStruct_022d3b8c_q unk134;
} UnkStruct_022d3b8c_o;

typedef struct UnkStruct_022d3b8c UnkStruct_022d3b8c;
struct UnkStruct_022d3b8c {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    u8 pad0[0x5c - 0x18];
    u16 unk5C;
    u8 pad0b[0x68 - 0x5e];
    void (*func)(UnkStruct_022d3b8c *s);
    u8 pad1[0x20c - 0x6c];
    u32 unk20C;
    u8 pad2[0xa9c - 0x210];
    UnkStruct_022d3b8c_a unkA9C;
    UnkStruct_022d3b8c_a unkBD4;
    UnkStruct_022d3b8c_a unkD0C;
    u8 pad3[0xf7c - 0xe44];
    s32 unkF7C;
    u8 pad3b[0xf84 - 0xf80];
    UnkStruct_022d3b8c_h unkF84[10];
    u8 pad4[0x12f4 - 0x113c];
    s32 unk12F4;
    u8 pad4b[0x1304 - 0x12f8];
    u16 unk1304;
    u8 pad5[0x131c - 0x1306];
    UnkStruct_022d3b8c_o *unk131C;
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

typedef struct UnkStruct_022d3b8c_p {
    u8 pad[0x10];
    s32 unk10;
} UnkStruct_022d3b8c_p;

typedef struct UnkStruct_022d3b8c_7 {
    u8 pad[8];
    u16 unk8;
    u16 unkA;
} UnkStruct_022d3b8c_7;

extern UnkStruct_022d3b8c *data_ov000_02369ae0;
extern UnkStruct_022d3b8c_p *gPlayers;
extern UnkStruct_022d3b8c_7 data_ov000_02369ac0;
extern u32 gRandSeed;
extern s32 data_ov000_02362e50[4];

s32 func_02002094(s32 a, s32 b);
void func_ov000_022cedd8(UnkStruct_022d3b8c *s, s32 a);
void func_ov000_022d3a78(UnkStruct_022d3b8c *s);
void func_ov000_022d3994(UnkStruct_022d3b8c *s);

static inline s32 Lerp(s32 a, s32 b) {
    return func_02002094(data_ov000_02369ac0.unk8 * a + (data_ov000_02369ac0.unkA - data_ov000_02369ac0.unk8) * b, data_ov000_02369ac0.unkA);
}

static inline u16 Rand(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return (u16)(gRandSeed >> 16);
}

void func_ov000_022d3b8c(void) {
    UnkStruct_022d3b8c *s = data_ov000_02369ae0;
    s32 d = gPlayers->unk10;
    UnkStruct_022d3b8c_a *x = &s->unkA9C;
    UnkStruct_022d3b8c_a *y = &s->unkBD4;
    UnkStruct_022d3b8c_a *z = &s->unkD0C;
    s32 a;
    s32 b;
    if (d < s->unk10) {
        a = s->unk10 - d;
        b = d + 0x6cd98 - s->unk10;
    } else {
        a = s->unk10 + 0x6cd98 - d;
        b = d - s->unk10;
    }
    if (s->unk1304 == 0) {
        if (b < a) {
            s->func = func_ov000_022d3a78;
            s->unk146C = 0x27d;
            Model_SetAnimation(x, 0, s->unkF7C, 8, 0);
            Model_SetAnimation(y, 0, s->unkF7C, 0x28, 0);
            Model_SetAnimation(z, 0, s->unkF7C, 0x44, 0);
            s->unk1460 = Lerp(0xc, 0x14);
            s->unk1464 = Lerp(0x780, 0xa00);
        } else {
            s->func = func_ov000_022d3994;
            s->unk146C = 0;
            Model_SetAnimation(x, 0, s->unkF7C, 5, 0);
            Model_SetAnimation(y, 0, s->unkF7C, 0x25, 0);
            Model_SetAnimation(z, 0, s->unkF7C, 0x41, 0);
            s->unk1460 = -Lerp(0xc, 0x14);
            s->unk1464 = -Lerp(0x780, 0xa00);
        }
    } else if (s->unk1304 != 0) {
        if (a < b) {
            s->func = func_ov000_022d3a78;
            s->unk146C = -0x27d;
            Model_SetAnimation(x, 0, s->unkF7C, 5, 0);
            Model_SetAnimation(y, 0, s->unkF7C, 0x25, 0);
            Model_SetAnimation(z, 0, s->unkF7C, 0x41, 0);
            s->unk1460 = -Lerp(0xc, 0x14);
            s->unk1464 = -Lerp(0x780, 0xa00);
        } else {
            s->func = func_ov000_022d3994;
            s->unk146C = 0;
            Model_SetAnimation(x, 0, s->unkF7C, 8, 0);
            Model_SetAnimation(y, 0, s->unkF7C, 0x28, 0);
            Model_SetAnimation(z, 0, s->unkF7C, 0x44, 0);
            s->unk1460 = Lerp(0xc, 0x14);
            s->unk1464 = Lerp(0x780, 0xa00);
        }
    }
    x->unk10C &= ~4;
    y->unk10C &= ~4;
    z->unk10C &= ~4;
    x->unk114 = 0x1000;
    y->unk114 = 0x1000;
    z->unk114 = 0x1000;
    func_ov000_022cedd8(s, 0x1c71000);
    s->unkF84[6].unkA &= ~4;
    s->unkF84[6].unkA |= 0x1000;
    s->unk12F4 = data_ov000_02362e50[Rand() & 3];
    s->unk1470 = 0;
    s->unk145C = 0;
    s->unk1468 = 0;
    s->unk1474 = s->unk1464;
}
