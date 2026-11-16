#include <nitro/types.h>

typedef struct UnkStruct_022d0864_a {
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
} UnkStruct_022d0864_a;

typedef struct UnkStruct_022d0864_h {
    u8 pad[0xa];
    u16 unkA;
    u8 pad1[0x2c - 0xc];
} UnkStruct_022d0864_h;

typedef struct UnkStruct_022d0864 UnkStruct_022d0864;
struct UnkStruct_022d0864 {
    u8 pad[0x10];
    s32 unk10;
    u8 pad0[0x68 - 0x14];
    void (*func)(UnkStruct_022d0864 *s);
    u8 pad1[0x20c - 0x6c];
    u32 unk20C;
    u8 pad2[0xa9c - 0x210];
    UnkStruct_022d0864_a unkA9C;
    UnkStruct_022d0864_a unkBD4;
    UnkStruct_022d0864_a unkD0C;
    u8 pad3[0xf7c - 0xe44];
    s32 unkF7C;
    u8 pad3b[0xf84 - 0xf80];
    UnkStruct_022d0864_h unkF84[10];
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
void func_ov000_022cd1b0(u32 flags);

typedef struct UnkStruct_022d0864_7 {
    u8 pad[8];
    u16 unk8;
    u16 unkA;
} UnkStruct_022d0864_7;

extern UnkStruct_022d0864_7 data_ov000_02369ac0;
extern s16 data_0206f470[];
extern s16 data_02071470[];

s32 func_02002094(s32 a, s32 b);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void func_ov000_022c5e1c(s32 unused, s32 len, s32 x, s32 y, s32 *out);
void func_ov000_022c5e80(s32 len, s32 b, s32 a, s32 *outX, s32 *outY, u16 *outAng);
u8 *func_ov000_02328548(s32 a, s32 x, s32 y, s32 z);
void func_02000e98(void *m, s32 s, s32 c);
void func_ov000_022d0690(UnkStruct_022d0864 *s);

void func_ov000_022d0864(UnkStruct_022d0864 *s) {
    UnkStruct_022d0864_a *x;
    UnkStruct_022d0864_a *y;
    UnkStruct_022d0864_a *z;
    s32 v;
    if (s->unkA9C.unk10C & 2) {
        s->func = func_ov000_022d0690;
        y = &s->unkBD4;
        x = &s->unkA9C;
        z = &s->unkD0C;
        v = 0x12d000 - *y->unkE4;
        if (s->unk145C) {
            Model_SetAnimation(x, 0, s->unkF7C, 0x1a, 0);
            Model_SetAnimation(y, 0, s->unkF7C, 0x37, 0);
            Model_SetAnimation(z, 0, s->unkF7C, 0x56, 0);
        } else {
            Model_SetAnimation(x, 0, s->unkF7C, 0x14, 0);
            Model_SetAnimation(y, 0, s->unkF7C, 0x33, 0);
            Model_SetAnimation(z, 0, s->unkF7C, 0x50, 0);
        }
        x->unk10C &= ~4;
        y->unk10C &= ~4;
        z->unk10C &= ~4;
        z->unk114 = func_02002094(data_ov000_02369ac0.unk8 * 0x666 + (data_ov000_02369ac0.unkA - data_ov000_02369ac0.unk8) * 0xb33, data_ov000_02369ac0.unkA);
        y->unk114 = z->unk114;
        x->unk114 = y->unk114;
        *y->unkE4 = v;
        s->unk12F4 = 200;
        Sound_PlaySeqArc(s->unk1458, 0, 0xa4);
        return;
    }
    if (s->unk12F4 > 0) {
        s->unk12F4--;
        if (s->unk12F4 > 0)
            return;
        s->unk12F4 = -1;
        s->unkA9C.unk114 = s->unkBD4.unk114 = s->unkD0C.unk114 = 0x1000;
        return;
    }
    if (*s->unkD0C.unkE4 < 0x9000)
        return;
    if (s->unk12F4 != 0)
        return;
    s->unkF84[5].unkA |= 4;
    s->unkF84[5].unkA &= ~0x1000;
    s->unkF84[3].unkA &= ~4;
    s->unkF84[3].unkA |= 0x1000;
    if (!(s->unk20C & 0x40)) {
        s32 a;
        s32 b;
        u8 *obj;
        s->unk20C |= 0x40;
        func_ov000_022c5e1c(0x1152f0, 0x6cd988, s->unk139C, s->unk13A4, &a);
        if (s->unk145C == 0)
            a -= 0x50000;
        else
            a += 0x50000;
        if (a < 0x40000)
            a = 0x70d988 - (0x40000 - a);
        else if (a > 0x70d988)
            a += -0x6cd988;
        func_ov000_022c5e80(0x1152f0, 0x6cd988, a, &a, &b, NULL);
        obj = func_ov000_02328548(1, a >> 4, -s->unk13A0 >> 4, b >> 4);
        if (s->unk145C == 0)
            func_02000e98(obj + 0x158, data_0206f470[0], data_0206f470[1]);
        else
            func_02000e98(obj + 0x158, data_02071470[0], data_02071470[1]);
        Sound_PlaySeqArc(0, 0, 0xa3);
    }
    s->unkA9C.unk114 = s->unkBD4.unk114 = s->unkD0C.unk114 = 0;
    s->unk12F4 = func_02002094(data_ov000_02369ac0.unk8 * 0x3c + (data_ov000_02369ac0.unkA - data_ov000_02369ac0.unk8) * 0x1e, data_ov000_02369ac0.unkA);
}
