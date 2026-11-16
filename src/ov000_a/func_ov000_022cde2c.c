#include <nitro/types.h>

typedef struct UnkStruct_022cde2c_h {
    s32 unk0;
    s32 unk4;
    s16 unk8;
    u16 unkA;
    u8 pad[0x2c - 0xc];
} UnkStruct_022cde2c_h;

typedef struct UnkStruct_022cde2c {
    u8 pad[0x14];
    s32 unk14;
    u8 padb[0x5c - 0x18];
    u16 unk5C;
    u8 pad0[0xf84 - 0x5e];
    UnkStruct_022cde2c_h unkF84[10];
    UnkStruct_022cde2c_h unk113C[10];
    u8 pad2[0x1348 - 0x12f4];
    s32 unk1348;
    u8 pad2b[4];
    s32 unk1350;
    u8 pad3[0x139c - 0x1354];
    s32 unk139C;
    s32 unk13A0;
    s32 unk13A4;
    u8 pad4[0x144c - 0x13a8];
    s32 unk144C;
} UnkStruct_022cde2c;

typedef struct UnkStruct_022cde2c_g {
    u8 pad[0x1444];
    u16 *unk1444;
    u16 *unk1448;
} UnkStruct_022cde2c_g;

typedef struct UnkStruct_022cde2c_2 {
    u8 pad[0x24];
    s32 unk24;
} UnkStruct_022cde2c_2;

extern UnkStruct_022cde2c_g *data_ov000_02369ae0;
extern UnkStruct_022cde2c_2 data_ov000_02369ae8;
extern u16 data_ov000_02362dec[];
extern u32 data_ov000_0236510c[];
extern u16 data_ov000_02362dcc[];

void func_ov000_022c5e1c(s32 unused, s32 len, s32 x, s32 y, s32 *out);
void ObjRect_Register(UnkStruct_022cde2c_h *h);
void func_ov000_022c5d4c(s32 a, u32 *p1, u16 *p2, u16 *p3, u16 n);

void func_ov000_022cde2c(UnkStruct_022cde2c *s) {
    s32 out;
    s32 i;
    UnkStruct_022cde2c_h *h1;
    UnkStruct_022cde2c_h *h2;
    s16 x;
    s16 xb;
    s16 y;
    s16 z;
    func_ov000_022c5e1c(0x1152f0, 0x6cd988, s->unk1348, s->unk1350, &out);
    {
        s16 x1;
        s16 xb1;
        s16 y1;
        x1 = out >> 12;
        y1 = s->unk14 >> 8;
        if (x1 < 0x366)
            xb1 = x1 + 0x6cd;
        else
            xb1 = x1 - 0x6cd;
        h1 = &s->unkF84[8];
        h2 = &s->unk113C[8];
        if (h1->unkA & 4) {
            h1->unk0 = x1;
            h2->unk0 = xb1;
            h1->unk4 = h2->unk4 = y1;
        h1->unk8 = h2->unk8 = 0;
        h1->unkA &= ~3;
        h2->unkA &= ~3;
        if (!(s->unk5C & 1)) {
            h1->unkA |= 1;
            h2->unkA |= 1;
        }
        ObjRect_Register(h1);
        ObjRect_Register(h2);
        }
    }
    func_ov000_022c5e1c(0x1152f0, 0x6cd988, s->unk139C, s->unk13A4, &out);
    z = 0;
    x = out >> 12;
    y = -(s->unk13A0 >> 12);
    if (x < 0x366)
        xb = x + 0x6cd;
    else
        xb = x - 0x6cd;
    for (i = 0; i < 3; i++) {
        h1 = &s->unkF84[data_ov000_02362dec[i]];
        h2 = &s->unk113C[data_ov000_02362dec[i]];
        if (h1->unkA & 4) {
            h1->unk0 = x;
            h2->unk0 = xb;
            h1->unk4 = h2->unk4 = y;
            h1->unk8 = h2->unk8 = z;
            h1->unkA &= ~3;
            h2->unkA &= ~3;
            if (!(s->unk5C & 1)) {
                h1->unkA |= 1;
                h2->unkA |= 1;
            }
            ObjRect_Register(h1);
            ObjRect_Register(h2);
        }
    }
    if (s->unkF84[5].unkA & 4) {
        s->unk144C++;
        if (s->unk144C == 3) {
            func_ov000_022c5d4c(data_ov000_02369ae8.unk24, data_ov000_0236510c, data_ov000_02369ae0->unk1448, data_ov000_02362dcc, 1);
        } else if (s->unk144C == 7) {
            func_ov000_022c5d4c(data_ov000_02369ae8.unk24, data_ov000_0236510c, data_ov000_02369ae0->unk1444, data_ov000_02362dcc, 1);
            s->unk144C = 0;
        }
    } else if (s->unk144C != 0) {
        func_ov000_022c5d4c(data_ov000_02369ae8.unk24, data_ov000_0236510c, data_ov000_02369ae0->unk1444, data_ov000_02362dcc, 1);
        s->unk144C = 0;
    }
}
