#include <nitro/types.h>

typedef struct UnkStruct_022d1dec_a {
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
} UnkStruct_022d1dec_a;

typedef struct UnkStruct_022d1dec_h {
    u8 pad[0xa];
    u16 unkA;
    u8 pad1[0x2c - 0xc];
} UnkStruct_022d1dec_h;

typedef struct UnkStruct_022d1dec_q {
    u8 pad[8];
    u16 unk8;
    u8 unkA;
    u8 unkB;
} UnkStruct_022d1dec_q;

typedef struct UnkStruct_022d1dec_o {
    u8 pad[0x54];
    u32 unk54;
    u8 pad0[0x5c - 0x58];
    u16 unk5C;
    u8 pad1[0x68 - 0x5e];
    void *unk68;
    u8 pad2[0x134 - 0x6c];
    UnkStruct_022d1dec_q unk134;
} UnkStruct_022d1dec_o;

typedef struct UnkStruct_022d1dec UnkStruct_022d1dec;
struct UnkStruct_022d1dec {
    u8 pad[0x10];
    s32 unk10;
    u8 pad0[0x68 - 0x14];
    void (*func)(UnkStruct_022d1dec *s);
    u8 pad1[0x20c - 0x6c];
    u32 unk20C;
    u8 pad2[0xa9c - 0x210];
    UnkStruct_022d1dec_a unkA9C;
    UnkStruct_022d1dec_a unkBD4;
    UnkStruct_022d1dec_a unkD0C;
    u8 pad3[0xf7c - 0xe44];
    s32 unkF7C;
    u8 pad3b[0xf84 - 0xf80];
    UnkStruct_022d1dec_h unkF84[10];
    u8 pad4[0x12f4 - 0x113c];
    s32 unk12F4;
    u8 pad4b[0x1304 - 0x12f8];
    u16 unk1304;
    u8 pad5[0x131c - 0x1306];
    UnkStruct_022d1dec_o *unk131C;
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
    u16 unk1460;
    u8 pad1460[2];
    u16 unk1464;
    u8 pad1464[2];
    u8 pad7[0x1470 - 0x1468];
    s32 unk1470;
    s32 unk1474;
};

void Model_SetAnimation(void *p, s32 a, s32 b, s32 c, s32 d);
void func_ov000_022cd1b0(u32 flags);

typedef struct UnkStruct_022d1dec_pl {
    u8 pad[0x10];
    s32 unk10;
} UnkStruct_022d1dec_pl;

extern UnkStruct_022d1dec_pl *gPlayers;

void func_ov000_022c5e80(s32 len, s32 b, s32 a, s32 *outX, s32 *outY, u16 *outAng);
UnkStruct_022d1dec_o *BossEffect_CreateSuction2(s32 a, s32 x, s32 y, s32 z);
void func_ov000_022d1cb0(UnkStruct_022d1dec *s);

void func_ov000_022d1dec(UnkStruct_022d1dec *s) {
    u16 ang;
    UnkStruct_022d1dec_q *q;
    UnkStruct_022d1dec_a *x;
    UnkStruct_022d1dec_a *y;
    UnkStruct_022d1dec_a *z;
    func_ov000_022c5e80(0x1152f0, 0x6cd988, gPlayers->unk10 << 4, NULL, NULL, &ang);
    if (s->unk145C >= 0x8000) {
        if ((u16)(ang - s->unk1304 + 0x71c) < 0x4000) {
            s->unk145C += s->unk1464 >> 3;
            if (s->unk145C > 0)
                s->unk145C = 0;
        }
    } else if (s->unk145C != 0) {
        if ((u16)(ang - s->unk1304 - 0x71c) > 0xc000) {
            s->unk145C -= s->unk1464 >> 3;
            if ((s16)s->unk145C < 0)
                s->unk145C = 0;
        }
    }
    if ((u16)(ang - s->unk1304 - 0x38e) < 0x8000) {
        s->unk145C += s->unk1460;
        if (s->unk145C > s->unk1464)
            s->unk145C = s->unk1464;
    } else if ((u16)(ang - s->unk1304 + 0x38e) > 0x8000) {
        s->unk145C -= s->unk1460;
        if (s->unk145C < (u16)-s->unk1464)
            s->unk145C = -s->unk1464;
    }
    s->unk1304 += s->unk145C;
    if (!(s->unkA9C.unk10C & 2))
        return;
    s->func = func_ov000_022d1cb0;
    y = &s->unkBD4;
    z = &s->unkD0C;
    x = &s->unkA9C;
    z->unk114 = 0;
    y->unk114 = z->unk114;
    x->unk114 = y->unk114;
    s->unk131C = BossEffect_CreateSuction2(1, s->unk139C >> 4, -s->unk13A0 >> 4, s->unk13A4 >> 4);
    q = &s->unk131C->unk134;
    s->unk131C->unk5C |= 4;
    s->unk131C->unk5C &= ~0x30;
    s->unk131C->unk68 = NULL;
    q->unk8 = 0x1a;
    q->unkA = 4;
    q->unkB = 2;
    s->unk1474 = 5;
}
