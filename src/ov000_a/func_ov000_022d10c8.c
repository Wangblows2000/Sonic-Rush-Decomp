#include <nitro/types.h>

typedef struct UnkStruct_022d10c8_a {
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
} UnkStruct_022d10c8_a;

typedef struct UnkStruct_022d10c8_h {
    u8 pad[0xa];
    u16 unkA;
    u8 pad1[0x2c - 0xc];
} UnkStruct_022d10c8_h;

typedef struct UnkStruct_022d10c8_q {
    u8 pad[8];
    u16 unk8;
    u8 unkA;
    u8 unkB;
} UnkStruct_022d10c8_q;

typedef struct UnkStruct_022d10c8_o {
    u8 pad[0x54];
    u32 unk54;
    u8 pad0[0x5c - 0x58];
    u16 unk5C;
    u8 pad1[0x68 - 0x5e];
    void *unk68;
    u8 pad2[0x134 - 0x6c];
    UnkStruct_022d10c8_q unk134;
} UnkStruct_022d10c8_o;

typedef struct UnkStruct_022d10c8 UnkStruct_022d10c8;
struct UnkStruct_022d10c8 {
    u8 pad[0x10];
    s32 unk10;
    u8 pad0[0x68 - 0x14];
    void (*func)(UnkStruct_022d10c8 *s);
    u8 pad1[0x20c - 0x6c];
    u32 unk20C;
    u8 pad2[0xa9c - 0x210];
    UnkStruct_022d10c8_a unkA9C;
    UnkStruct_022d10c8_a unkBD4;
    UnkStruct_022d10c8_a unkD0C;
    u8 pad3[0xf7c - 0xe44];
    s32 unkF7C;
    u8 pad3b[0xf84 - 0xf80];
    UnkStruct_022d10c8_h unkF84[10];
    u8 pad4[0x12f4 - 0x113c];
    s32 unk12F4;
    u8 pad4b[0x1304 - 0x12f8];
    u16 unk1304;
    u8 pad5[0x131c - 0x1306];
    UnkStruct_022d10c8_o *unk131C;
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

void func_02031a68(void *p, s32 a, s32 b, s32 c, s32 d);
void func_ov000_022cd1b0(u32 flags);

typedef struct UnkStruct_022d10c8_7 {
    u8 pad[8];
    u16 unk8;
    u16 unkA;
} UnkStruct_022d10c8_7;

extern UnkStruct_022d10c8_7 data_ov000_02369ac0;

s32 func_02002094(s32 a, s32 b);
void func_02036754(s32 a, s32 b, s32 c);
UnkStruct_022d10c8_o *func_ov000_02328510(s32 a, s32 x, s32 y, s32 z);
void func_ov000_022d0e9c(UnkStruct_022d10c8 *s);

void func_ov000_022d10c8(UnkStruct_022d10c8 *s) {
    UnkStruct_022d10c8_a *x;
    UnkStruct_022d10c8_a *y;
    UnkStruct_022d10c8_a *z;
    UnkStruct_022d10c8_q *q;
    if (!(s->unkA9C.unk10C & 2))
        return;
    s->func = func_ov000_022d0e9c;
    x = &s->unkA9C;
    y = &s->unkBD4;
    z = &s->unkD0C;
    if (s->unk145C) {
        func_02031a68(x, 0, s->unkF7C, 0x18, 0);
        func_02031a68(y, 0, s->unkF7C, 0x36, 0);
        func_02031a68(z, 0, s->unkF7C, 0x54, 0);
    } else {
        func_02031a68(x, 0, s->unkF7C, 0x12, 0);
        func_02031a68(y, 0, s->unkF7C, 0x32, 0);
        func_02031a68(z, 0, s->unkF7C, 0x4e, 0);
    }
    x->unk10C &= ~4;
    y->unk10C &= ~4;
    z->unk10C &= ~4;
    z->unk114 = func_02002094(data_ov000_02369ac0.unk8 * 0xb33 + (data_ov000_02369ac0.unkA - data_ov000_02369ac0.unk8) * 0x1800, data_ov000_02369ac0.unkA);
    y->unk114 = z->unk114;
    x->unk114 = y->unk114;
    s->unk20C |= 4;
    y->unkB = 0;
    s->unkF84[3].unkA |= 4;
    s->unkF84[3].unkA &= ~0x1000;
    s->unkF84[9].unkA |= 4;
    s->unkF84[9].unkA &= ~0x1000;
    func_02036754(s->unk1458, 0, 0xa4);
    s->unk131C = func_ov000_02328510(1, s->unk139C >> 4, -s->unk13A0 >> 4, s->unk13A4 >> 4);
    q = &s->unk131C->unk134;
    s->unk131C->unk5C |= 4;
    s->unk131C->unk5C &= ~0x30;
    s->unk131C->unk68 = NULL;
    q->unk8 = 0x1b;
    q->unkA = 4;
    q->unkB = 2;
}
