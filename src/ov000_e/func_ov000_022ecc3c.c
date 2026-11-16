#include <nitro/types.h>

typedef struct UnkStruct_022ecc3c_3 {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 padx[0x54 - 0x1c];
    u32 unk54;
    u8 pad1[0x220 - 0x58];
    s32 unk220;
    u8 pad2[4];
    u8 unk228[0x33c - 0x228];
    s32 unk33C;
} UnkStruct_022ecc3c_3;

typedef struct UnkStruct_022ecc3c_4 {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
    u8 pad[0x54 - 0x18];
    u32 unk54;
    u8 pad1[0x304 - 0x58];
    s32 unk304;
    u8 pad2[4];
    u8 unk30C[0x330 - 0x30c];
    u8 unk330[0x420 - 0x330];
    s32 unk420;
} UnkStruct_022ecc3c_4;

typedef struct UnkStruct_022ecc3c UnkStruct_022ecc3c;
struct UnkStruct_022ecc3c {
    u8 pad[0x54];
    u32 unk54;
    u8 pad0[0x68 - 0x58];
    void (*func)(UnkStruct_022ecc3c *s);
    u8 pad1[0x20c - 0x6c];
    u32 unk20C;
    UnkStruct_022ecc3c_3 *unk210;
    UnkStruct_022ecc3c_4 *unk214;
    u8 pad2[0x220 - 0x218];
    u16 unk220;
    u8 pad3[0x230 - 0x222];
    u16 unk230;
    s32 unk350;
};

typedef struct UnkStruct_022ecc3c_cam {
    u8 pad[0xdc];
    s32 unkDC;
} UnkStruct_022ecc3c_cam;

typedef struct UnkStruct_022ecc3c_c {
    u8 pad[0x30];
    u16 unk30;
    u8 pad0[0x3c - 0x32];
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    s32 unk54;
    s32 unk58;
    s32 unk5C;
    u8 pad1[0x6c - 0x60];
    u16 unk6C;
    u8 pad2[0x78 - 0x6e];
    s32 unk78;
    u8 pad3[0x80 - 0x7c];
    u16 unk80;
    u8 pad4[0x84 - 0x82];
    s32 unk84;
    u8 pad5[0x8c - 0x88];
    u16 unk8C;
    u16 unk8E;
    u8 pad6[0x92 - 0x90];
    u16 unk92;
} UnkStruct_022ecc3c_c;

typedef struct UnkStruct_022ecc3c_pl {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 padq[0x5c - 0x1c];
    u16 unk5C;
    u8 pad1[0x32c - 0x5e];
    u32 unk32C;
    u8 pad2[0x420 - 0x330];
    u16 unk420;
    u16 unk422;
    u16 unk424;
} UnkStruct_022ecc3c_pl;

typedef struct UnkStruct_022ecc3c_5 {
    u8 pad[0x48];
    s16 unk48;
} UnkStruct_022ecc3c_5;

extern UnkStruct_022ecc3c_cam data_ov000_0236a444;
extern UnkStruct_022ecc3c_c data_ov000_0236a48c;
extern UnkStruct_022ecc3c_pl *data_ov000_0236ac60;
extern UnkStruct_022ecc3c_5 data_0208922c;

void func_ov000_022c5c80(u32 id);
void func_02036754(s32 a, s32 b, s32 c);
void func_ov000_022ecc00(UnkStruct_022ecc3c *s);

void func_ov000_022ecc3c(UnkStruct_022ecc3c *s) {
    UnkStruct_022ecc3c_c *c;
    UnkStruct_022ecc3c_4 *o = s->unk214;
    data_ov000_0236a444.unkDC = 2;
    c = (UnkStruct_022ecc3c_c *)(u32)&data_ov000_0236a48c;
    c->unk30 = 0x1000;
    c->unk3C = c->unk40 = c->unk44 = 0;
    c->unk48 = c->unk4C = c->unk50 = 0;
    c->unk54 = (data_ov000_0236ac60->unk10 + o->unk10) << 3;
    c->unk58 = -o->unk14 << 4;
    c->unk5C = 0;
    c->unk6C = 0x1000;
    c->unk78 = 0x50000;
    c->unk80 = 0x1000;
    c->unk84 = 0x20000;
    c->unk8C = 0x1000;
    c->unk8E = 0x2aaa;
    c->unk92 = 0x1000;
    func_ov000_022c5c80(6);
    data_0208922c.unk48 = 0x10;
    func_02036754(0, 0, 0x7c);
    s->unk230 = 0;
    s->func = func_ov000_022ecc00;
}
