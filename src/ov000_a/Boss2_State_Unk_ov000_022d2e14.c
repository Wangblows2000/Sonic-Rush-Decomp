#include <nitro/types.h>

typedef struct UnkStruct_022d2e14_a {
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
} UnkStruct_022d2e14_a;

typedef struct UnkStruct_022d2e14_h {
    u8 pad[0xa];
    u16 unkA;
    u8 pad1[0x2c - 0xc];
} UnkStruct_022d2e14_h;

typedef struct UnkStruct_022d2e14_q {
    u8 pad[8];
    u16 unk8;
    u8 unkA;
    u8 unkB;
} UnkStruct_022d2e14_q;

typedef struct UnkStruct_022d2e14_o {
    u8 pad[0x54];
    u32 unk54;
    u8 pad0[0x5c - 0x58];
    u16 unk5C;
    u8 pad1[0x68 - 0x5e];
    void *unk68;
    u8 pad2[0x134 - 0x6c];
    UnkStruct_022d2e14_q unk134;
} UnkStruct_022d2e14_o;

typedef struct UnkStruct_022d2e14 UnkStruct_022d2e14;
struct UnkStruct_022d2e14 {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    u8 pad0[0x5c - 0x18];
    u16 unk5C;
    u8 pad0b[0x68 - 0x5e];
    void (*func)(UnkStruct_022d2e14 *s);
    u8 pad1[0x20c - 0x6c];
    u32 unk20C;
    u8 pad2[0xa9c - 0x210];
    UnkStruct_022d2e14_a unkA9C;
    UnkStruct_022d2e14_a unkBD4;
    UnkStruct_022d2e14_a unkD0C;
    u8 pad3[0xf7c - 0xe44];
    s32 unkF7C;
    u8 pad3b[0xf84 - 0xf80];
    UnkStruct_022d2e14_h unkF84[10];
    u8 pad4[0x12f4 - 0x113c];
    s32 unk12F4;
    u8 pad4b[0x1304 - 0x12f8];
    u16 unk1304;
    u8 pad5[0x131c - 0x1306];
    UnkStruct_022d2e14_o *unk131C;
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

typedef struct UnkStruct_022d2e14_c {
    u8 pad[0x3c];
    s32 unk3C;
    s32 unk40;
} UnkStruct_022d2e14_c;

extern UnkStruct_022d2e14_c data_ov000_0236a444;

s32 ScreenShake_GetOffsetX(void);
s32 ScreenShake_GetOffsetY(void);
void ScreenShake_Start(s32 a);
s32 FX_Div(s32 a, s32 b);
void Boss2_State_WaitAnimThenSelectMode1(UnkStruct_022d2e14 *s);

void Boss2_State_Unk_ov000_022d2e14(UnkStruct_022d2e14 *s) {
    UnkStruct_022d2e14_a *x = &s->unkA9C;
    UnkStruct_022d2e14_a *y = &s->unkBD4;
    UnkStruct_022d2e14_a *z = &s->unkD0C;
    s32 k;
    if (s->unk12F4 >= 0) {
        k = s->unk12F4 << 12;
        data_ov000_0236a444.unk3C = FX_Div(k * ScreenShake_GetOffsetX(), 0xf000);
        data_ov000_0236a444.unk3C = data_ov000_0236a444.unk3C << 12;
        data_ov000_0236a444.unk3C = data_ov000_0236a444.unk3C >> 12;
        data_ov000_0236a444.unk40 = FX_Div(k * ScreenShake_GetOffsetY(), 0xf000);
        data_ov000_0236a444.unk40 = data_ov000_0236a444.unk40 << 12;
        data_ov000_0236a444.unk40 = data_ov000_0236a444.unk3C >> 12;
        s->unk12F4--;
    }
    if (!(x->unk10C & 2))
        return;
    s->func = Boss2_State_WaitAnimThenSelectMode1;
    Model_SetAnimation(x, 0, s->unkF7C, 0x21, 0);
    Model_SetAnimation(y, 0, s->unkF7C, 0x3d, 0);
    Model_SetAnimation(z, 0, s->unkF7C, 0x5d, 0);
    x->unk114 = 0x1000;
    y->unk114 = 0x1000;
    z->unk114 = 0x1000;
    ScreenShake_Start(0);
}
