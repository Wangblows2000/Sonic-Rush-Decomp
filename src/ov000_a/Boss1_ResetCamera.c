#include <nitro/types.h>

typedef struct UnkStruct_022cc224_cam {
    u8 pad[0xc];
    s32 unkC;
    u8 pad0[0x14 - 0x10];
    s32 unk14;
    u8 pad1[0xdc - 0x18];
    s32 unkDC;
} UnkStruct_022cc224_cam;

typedef struct UnkStruct_022cc224_c {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    u8 pad0[0x30 - 0x18];
    u16 unk30;
    u8 pad1[0x3c - 0x32];
    void *unk3C;
    s32 unk40;
    void *unk44;
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    u8 pad2[0x58 - 0x54];
    s32 unk58;
    u8 pad3[0x6c - 0x5c];
    u16 unk6C;
    u8 pad4[0x78 - 0x6e];
    s32 unk78;
    s32 unk7C;
    u16 unk80;
    u8 pad5[0x84 - 0x82];
    s32 unk84;
    s32 unk88;
    u16 unk8C;
    u16 unk8E;
    u16 unk90;
    u16 unk92;
} UnkStruct_022cc224_c;

typedef struct UnkStruct_022cc224_pl {
    u8 pad[0x430];
    u16 unk430;
    u16 unk432;
} UnkStruct_022cc224_pl;

extern UnkStruct_022cc224_cam data_ov000_0236a444;
extern UnkStruct_022cc224_c data_ov000_0236a48c;
extern UnkStruct_022cc224_pl *gPlayers;

s32 FX_Div(s32 a, s32 b);

void Boss1_ResetCamera(void) {
    UnkStruct_022cc224_c *c;
    *(vu16 *)0x4000304 |= 0x8000;
    data_ov000_0236a444.unkDC = 2;
    data_ov000_0236a444.unkC = 0x1a66000;
    c = (UnkStruct_022cc224_c *)(u32)&data_ov000_0236a48c;
    data_ov000_0236a444.unk14 = FX_Div(0x1000, 0x34cc);
    c->unk0 = c->unk4 = c->unk8 = 0;
    c->unkC = c->unk10 = c->unk14 = 0;
    c->unk30 = 0x200;
    c->unk3C = gPlayers;
    c->unk40 = 0;
    c->unk44 = gPlayers;
    c->unk58 = -0x1c0000;
    c->unk48 = 0;
    c->unk4C = 0x28000;
    c->unk50 = 0;
    c->unk6C = 0x400;
    c->unk78 = c->unk7C = 0xa0000;
    c->unk80 = 0x200;
    c->unk84 = c->unk88 = -0x14000;
    c->unk8C = 0x80;
    c->unk8E = c->unk90 = 0;
    c->unk92 = 0x100;
    gPlayers->unk430 = 0x40;
    gPlayers->unk432 = 0x80;
}
