#include <nitro/types.h>

typedef struct UnkStruct_022d1cb0_a {
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
    u8 pad3[0x12c - 0x118];
    s16 unk12C;
    u8 pad4[0x138 - 0x12e];
} UnkStruct_022d1cb0_a;

typedef struct UnkStruct_022d1cb0_h {
    u8 pad[0xa];
    u16 unkA;
    u8 pad1[0x2c - 0xc];
} UnkStruct_022d1cb0_h;

typedef struct UnkStruct_022d1cb0_q {
    u8 pad[8];
    u16 unk8;
    u8 unkA;
    u8 unkB;
} UnkStruct_022d1cb0_q;

typedef struct UnkStruct_022d1cb0_o {
    u8 pad[0x54];
    u32 unk54;
    u8 pad0[0x5c - 0x58];
    u16 unk5C;
    u8 pad1[0x68 - 0x5e];
    void *unk68;
    u8 pad2[0x134 - 0x6c];
    UnkStruct_022d1cb0_q unk134;
} UnkStruct_022d1cb0_o;

typedef struct UnkStruct_022d1cb0 UnkStruct_022d1cb0;
struct UnkStruct_022d1cb0 {
    u8 pad[0x10];
    s32 unk10;
    u8 pad0[0x68 - 0x14];
    void (*func)(UnkStruct_022d1cb0 *s);
    u8 pad1[0x20c - 0x6c];
    u32 unk20C;
    u8 pad2[0xa9c - 0x210];
    UnkStruct_022d1cb0_a unkA9C;
    UnkStruct_022d1cb0_a unkBD4;
    UnkStruct_022d1cb0_a unkD0C;
    u8 pad3[0xf7c - 0xe44];
    s32 unkF7C;
    u8 pad3b[0xf84 - 0xf80];
    UnkStruct_022d1cb0_h unkF84[10];
    u8 pad4[0x12f4 - 0x113c];
    s32 unk12F4;
    u8 pad4b[0x1304 - 0x12f8];
    u16 unk1304;
    u8 pad5[0x131c - 0x1306];
    UnkStruct_022d1cb0_o *unk131C;
    u8 pad5b[0x139c - 0x1320];
    s32 unk139C;
    s32 unk13A0;
    s32 unk13A4;
    u8 pad5c[0x1450 - 0x13a8];
    s32 unk1450;
    u8 pad5d[0x1458 - 0x1454];
    s32 unk1458;
    u16 unk145C;
    u8 pad6[0x1460 - 0x145e];
    s32 unk1460;
    s32 unk1464;
    u8 pad7[0x1470 - 0x1468];
    s32 unk1470;
    s32 unk1474;
};

void Model_SetAnimation(void *p, s32 a, s32 b, s32 c, s32 d);
void func_ov000_022cd1b0(u32 flags);

typedef struct UnkStruct_022d1cb0_7 {
    u8 pad[8];
    u16 unk8;
    u16 unkA;
} UnkStruct_022d1cb0_7;

extern UnkStruct_022d1cb0_7 data_ov000_02369ac0;

s32 func_02002094(s32 a, s32 b);
void func_020366f4(s32 a, s32 b, s32 c);
void func_ov000_022d1ab0(UnkStruct_022d1cb0 *s);

static inline s32 Lerp(s32 a, s32 b) {
    return func_02002094(data_ov000_02369ac0.unk8 * a + (data_ov000_02369ac0.unkA - data_ov000_02369ac0.unk8) * b, data_ov000_02369ac0.unkA);
}

void func_ov000_022d1cb0(UnkStruct_022d1cb0 *s) {
    UnkStruct_022d1cb0_a *x;
    UnkStruct_022d1cb0_a *y;
    UnkStruct_022d1cb0_a *z;
    s->unk1474--;
    if (s->unk1474 > 0)
        return;
    s->func = func_ov000_022d1ab0;
    x = &s->unkA9C;
    y = &s->unkBD4;
    z = &s->unkD0C;
    Model_SetAnimation(x, 0, s->unkF7C, 0xd, 0);
    Model_SetAnimation(y, 0, s->unkF7C, 0x2d, 0);
    Model_SetAnimation(z, 0, s->unkF7C, 0x49, 0);
    x->unk10C &= ~4;
    y->unk10C &= ~4;
    z->unk10C &= ~4;
    z->unk114 = Lerp(0x1000, 0x1800);
    y->unk114 = z->unk114;
    x->unk114 = y->unk114;
    s->unkF84[1].unkA |= 4;
    s->unkF84[1].unkA &= ~0x1000;
    func_020366f4(0, 0, 0xd0);
}
