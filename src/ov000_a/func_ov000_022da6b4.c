#include <nitro/types.h>

typedef struct UnkStruct_022da6b4_b {
    u8 pad[0x210];
    s32 unk210;
} UnkStruct_022da6b4_b;

typedef struct UnkStruct_022da6b4_d {
    u8 pad[0xc];
    s32 unkC;
    u8 pad1[0x14 - 0x10];
    s32 unk14;
    u8 pad2[0xdc - 0x18];
    s32 unkDC;
} UnkStruct_022da6b4_d;

typedef struct UnkStruct_022da6b4_c {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    u8 pad[0x30 - 0x18];
    u16 unk30;
    u8 pad1[0x38 - 0x32];
    s32 unk38;
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    s32 unk54;
    s32 unk58;
    s32 unk5C;
    u8 pad2[0x6c - 0x60];
    u16 unk6C;
    u8 pad3[0x74 - 0x6e];
    s32 unk74;
    s32 unk78;
    s32 unk7C;
    u16 unk80;
    u8 pad4[0x84 - 0x82];
    s32 unk84;
    s32 unk88;
    u16 unk8C;
    u16 unk8E;
    u16 unk90;
    u16 unk92;
} UnkStruct_022da6b4_c;

extern UnkStruct_022da6b4_b *data_ov000_02369b14;
extern UnkStruct_022da6b4_d data_ov000_0236a444;
extern UnkStruct_022da6b4_c data_ov000_0236a48c;

s32 func_0200225c(s32 a, s32 b);

void func_ov000_022da6b4(void) {
    UnkStruct_022da6b4_c *c;
    data_ov000_02369b14->unk210 = 0;
    data_ov000_0236a444.unkDC = 2;
    data_ov000_0236a444.unkC = 0x1a66000;
    c = (UnkStruct_022da6b4_c *)(u32)&data_ov000_0236a48c;
    data_ov000_0236a444.unk14 = func_0200225c(0x1000, 0x34cc);
    c->unk8 = 0;
    c->unk4 = c->unk8;
    c->unk0 = c->unk4;
    c->unk14 = 0;
    c->unk10 = c->unk14;
    c->unkC = c->unk10;
    c->unk30 = 0x200;
    c->unk38 = 0;
    c->unk3C = 0;
    c->unk40 = 0;
    c->unk44 = 0;
    c->unk54 = 0;
    c->unk58 = -0x200000;
    c->unk5C = 0;
    c->unk48 = 0;
    c->unk4C = 0x28000;
    c->unk50 = 0;
    c->unk6C = 0x200;
    c->unk74 = 0;
    c->unk7C = 0x1a82f8;
    c->unk78 = c->unk7C;
    c->unk80 = 0x10;
    c->unk88 = 0x30000;
    c->unk84 = c->unk88;
    c->unk8C = 0x10;
    c->unk90 = 0;
    c->unk8E = c->unk90;
    c->unk92 = 0xccc;
}
