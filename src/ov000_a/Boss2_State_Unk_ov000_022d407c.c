#include <nitro/types.h>

typedef struct UnkStruct_022d407c_a {
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
    u8 padE8[0xf8 - 0xe8];
    s32 *unkF8;
    u8 padFC[0x10c - 0xfc];
    u8 unk10C;
    u8 pad2[0x114 - 0x10d];
    s32 unk114;
    u8 pad3[0x12c - 0x118];
    s16 unk12C;
    u8 pad4[0x138 - 0x12e];
} UnkStruct_022d407c_a;

typedef struct UnkStruct_022d407c_h {
    u8 pad[0xa];
    u16 unkA;
    u8 pad1[0x2c - 0xc];
} UnkStruct_022d407c_h;

typedef struct UnkStruct_022d407c_q {
    u8 pad[8];
    u16 unk8;
    u8 unkA;
    u8 unkB;
} UnkStruct_022d407c_q;

typedef struct UnkStruct_022d407c_o {
    u8 pad[0x54];
    u32 unk54;
    u8 pad0[0x5c - 0x58];
    u16 unk5C;
    u8 pad1[0x68 - 0x5e];
    void *unk68;
    u8 pad2[0x134 - 0x6c];
    UnkStruct_022d407c_q unk134;
} UnkStruct_022d407c_o;

typedef struct UnkStruct_022d407c UnkStruct_022d407c;
struct UnkStruct_022d407c {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    u8 pad0[0x5c - 0x18];
    u16 unk5C;
    u8 pad0b[0x68 - 0x5e];
    void (*func)(UnkStruct_022d407c *s);
    u8 pad1[0x20c - 0x6c];
    u32 unk20C;
    u8 pad2[0xa9c - 0x210];
    UnkStruct_022d407c_a unkA9C;
    UnkStruct_022d407c_a unkBD4;
    UnkStruct_022d407c_a unkD0C;
    u8 pad3[0xf7c - 0xe44];
    s32 unkF7C;
    u8 pad3b[0xf84 - 0xf80];
    UnkStruct_022d407c_h unkF84[10];
    u8 pad4[0x12f4 - 0x113c];
    s32 unk12F4;
    u8 pad4b[0x1304 - 0x12f8];
    u16 unk1304;
    u8 pad5[0x131c - 0x1306];
    UnkStruct_022d407c_o *unk131C;
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

typedef struct UnkStruct_022d407c_7 {
    u8 pad[0x62];
    u16 unk62;
} UnkStruct_022d407c_7;

extern UnkStruct_022d407c_7 gCamera;

void Boss2_State_Unk_ov000_022d4064(UnkStruct_022d407c *s);

void Boss2_State_Unk_ov000_022d407c(UnkStruct_022d407c *s) {
    UnkStruct_022d407c_a *x;
    UnkStruct_022d407c_a *y;
    UnkStruct_022d407c_a *z;
    if (!(gCamera.unk62 <= 459.5))
        return;
    s->func = Boss2_State_Unk_ov000_022d4064;
    x = &s->unkA9C;
    x->unk10C |= 0x40;
    y = &s->unkBD4;
    z = &s->unkD0C;
    y->unk10C |= 0x40;
    z->unk10C |= 0x40;
    Model_SetAnimation(x, 0, s->unkF7C, 4, 0);
    Model_SetAnimation(y, 0, s->unkF7C, 0x24, 0);
    Model_SetAnimation(z, 0, s->unkF7C, 0x40, 0);
    z->unk12C = 0x8c;
    y->unk12C = z->unk12C;
    x->unk12C = y->unk12C;
    *x->unkE4 = *x->unkF8;
    *y->unkE4 = *x->unkF8;
    *z->unkE4 = *x->unkF8;
}
