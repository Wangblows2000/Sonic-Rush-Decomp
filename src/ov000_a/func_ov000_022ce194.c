#include <nitro/types.h>

typedef struct UnkStruct_022ce194_h {
    s32 unk0;
    s32 unk4;
    s16 unk8;
    u16 unkA;
    u8 pad[0x2c - 0xc];
} UnkStruct_022ce194_h;

typedef struct UnkStruct_022ce194 {
    u8 pad[0x5c];
    u16 unk5C;
    u8 pad0[0xf84 - 0x5e];
    UnkStruct_022ce194_h unkF84[4];
    UnkStruct_022ce194_h unk1034;
    u8 pad1[0x113c - 0x1060];
    UnkStruct_022ce194_h unk113C[4];
    u8 pad2[0x1304 - 0x11ec];
    u16 unk1304;
    u8 pad3[0x139c - 0x1306];
    s32 unk139C;
    s32 unk13A0;
    s32 unk13A4;
    u8 pad4[0x144c - 0x13a8];
    s32 unk144C;
} UnkStruct_022ce194;

typedef struct UnkStruct_022ce194_g {
    u8 pad[0x1444];
    u16 *unk1444;
    u16 *unk1448;
} UnkStruct_022ce194_g;

typedef struct UnkStruct_022ce194_2 {
    u8 pad[0x24];
    s32 unk24;
} UnkStruct_022ce194_2;

extern UnkStruct_022ce194_g *data_ov000_02369ae0;
extern UnkStruct_022ce194_2 data_ov000_02369ae8;
extern u16 data_ov000_02362dfc[];
extern u32 data_ov000_0236510c[];
extern u16 data_ov000_02362dcc[];

s32 func_020021f0(s32 a);
void ObjRect_Register(UnkStruct_022ce194_h *h);
void func_ov000_022c5d4c(s32 a, u32 *p1, u16 *p2, u16 *p3, u16 n);

void func_ov000_022ce194(UnkStruct_022ce194 *s) {
    s32 d;
    s32 m;
    s32 i;
    UnkStruct_022ce194_h *h1;
    UnkStruct_022ce194_h *h2;
    s16 x;
    s16 xb;
    s16 y;
    s16 z;
    {
        s32 az = s->unk139C >> 12;
        s32 ax = s->unk13A4 >> 12;
        d = func_020021f0((az * az + ax * ax) << 12);
    }
    m = (s32)(((s64)((s->unk1304 << 12) >> 16) * 0x6cd98 + 0x800) >> 12);
    if (m < 0x4000)
        m += 0x6cd98;
    else if (m > 0x70d98)
        m -= 0x6cd98;
    x = m >> 8;
    y = -(s->unk13A0 >> 12);
    z = (-(0x1152f0 - d) >> 12) + 0x23;
    if (x < 0x366)
        xb = x + 0x6cd;
    else
        xb = x - 0x6cd;
    for (i = 0; i < 3; i++) {
        h1 = &s->unkF84[data_ov000_02362dfc[i]];
        h2 = &s->unk113C[data_ov000_02362dfc[i]];
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
    if (s->unk1034.unkA & 4) {
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
