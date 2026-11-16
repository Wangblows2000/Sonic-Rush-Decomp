#include <nitro/types.h>

typedef struct UnkStruct_022ce614_m {
    s32 m[9];
} UnkStruct_022ce614_m;

typedef struct UnkStruct_022ce614_a {
    u8 pad[0x24];
    UnkStruct_022ce614_m unk24;
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    u8 pad1[0x138 - 0x54];
} UnkStruct_022ce614_a;

typedef struct UnkStruct_022ce614_h {
    s32 unk0;
    s32 unk4;
    s16 unk8;
    u16 unkA;
    u8 pad[0x2c - 0xc];
} UnkStruct_022ce614_h;

typedef struct UnkStruct_022ce614 {
    u8 pad[6];
    s8 unk6;
    s8 unk7;
    u8 pad0[0x10 - 8];
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 pad1[0x5c - 0x1c];
    u16 unk5C;
    u8 pad2[0x20c - 0x5e];
    u32 unk20C;
    u8 pad2b[4];
    UnkStruct_022ce614_a unk214[7];
    UnkStruct_022ce614_a unkA9C;
    UnkStruct_022ce614_a unkBD4;
    UnkStruct_022ce614_a unkD0C;
    UnkStruct_022ce614_a unkE44;
    u8 pad3[0xf84 - 0xf7c];
    UnkStruct_022ce614_h unkF84[10];
    UnkStruct_022ce614_h unk113C[10];
    u8 pad4[0x1304 - 0x12f4];
    u16 unk1304;
    u8 pad5[0x1308 - 0x1306];
    s32 unk1308;
    s32 unk130C;
    s32 unk1310;
    s32 unk1314;
    u16 unk1318;
    u8 pad6[0x1324 - 0x131a];
    u8 unk1324[0x1354 - 0x1324];
    u8 unk1354[0x1378 - 0x1354];
    u8 unk1378[0x13a8 - 0x1378];
    u8 unk13A8[0x13cc - 0x13a8];
    u8 unk13CC[0x13f0 - 0x13cc];
    s32 unk13F0;
    s32 unk13F4;
    s32 unk13F8;
    u8 unk13FC[0x1420 - 0x13fc];
    void *unk1420;
} UnkStruct_022ce614;

typedef struct UnkStruct_022ce614_2 {
    u8 pad[0x10];
    UnkStruct_022ce614 *unk10;
} UnkStruct_022ce614_2;

typedef struct UnkStruct_022ce614_7 {
    u8 pad[4];
    s32 unk4;
} UnkStruct_022ce614_7;

extern UnkStruct_022ce614_2 *gTaskCurrent;
extern UnkStruct_022ce614_7 gBossStatus;
extern s16 data_0206e470[];

void Model_Animate(void *p);
void Model_Draw(void *p);
void func_ov000_022c5e80(s32 len, s32 b, s32 a, s32 *outX, s32 *outY, u16 *outAng);
void MTX_RotY33_(void *m, s32 s, s32 c);
void MTX_RotZ33_(void *m, s32 s, s32 c);
void NNS_G3dGeBufferOP_N(s32 a, void *p, s32 n);
void NNS_G3dGetCurrentMtx(void *a, void *b);
void func_ov000_022ce474(UnkStruct_022ce614 *s);
void func_ov000_022ce194(UnkStruct_022ce614 *s);
void func_ov000_022cde2c(UnkStruct_022ce614 *s);
void func_ov000_022c5e1c(s32 unused, s32 len, s32 x, s32 y, s32 *out);
void ObjRect_Register(UnkStruct_022ce614_h *h);

void Boss2_Draw_Unk_ov000_022ce614(void) {
    UnkStruct_022ce614 *s = gTaskCurrent->unk10;
    UnkStruct_022ce614_a *a;
    UnkStruct_022ce614_h *h1;
    UnkStruct_022ce614_h *h2;
    s32 out;
    s16 x;
    s16 xb;
    s16 y;
    if (!(s->unk20C & 1)) {
        s32 i;
        for (i = 0; i <= 2; i++) {
            Model_Animate(&s->unk214[i]);
            Model_Draw(&s->unk214[i]);
        }
    }
    if (s->unk1420 != NULL) {
        Model_Animate(s->unk1420);
        Model_Draw(s->unk1420);
    }
    if (s->unk5C & 0x20)
        return;
    a = &s->unkA9C;
    if (!(s->unk20C & 2)) {
        u16 ang;
        if (s->unk10 < 0x4000)
            s->unk10 += 0x6cd98;
        else if (s->unk10 > 0x70d98)
            s->unk10 -= 0x6cd98;
        func_ov000_022c5e80(0x1152f0, 0x6cd988, s->unk10 << 4, &a->unk48, &a->unk50, &ang);
        a->unk4C = -s->unk14 << 4;
        MTX_RotY33_(&a->unk24, data_0206e470[(((u16)(ang + 0xc000 + s->unk1304) & 0xffff) >> 4) * 2], data_0206e470[(((u16)(ang + 0xc000 + s->unk1304) & 0xffff) >> 4) * 2 + 1]);
    } else {
        a->unk48 = s->unk10 << 4;
        a->unk4C = -s->unk14 << 4;
        a->unk50 = s->unk18 << 4;
        MTX_RotY33_(&a->unk24, data_0206e470[((s->unk1304 & 0xffff) >> 4) * 2], data_0206e470[((s->unk1304 & 0xffff) >> 4) * 2 + 1]);
    }
    a->unk48 += s->unk6 << 12;
    a->unk4C += s->unk7 << 12;
    Model_Animate(a);
    Model_Draw(a);
    {
        u32 v = 2;
        NNS_G3dGeBufferOP_N(0x10, &v, 1);
    }
    {
        u32 v = 0x1e;
        NNS_G3dGeBufferOP_N(0x14, &v, 1);
    }
    NNS_G3dGetCurrentMtx(s->unk1324, s->unk1354);
    a = &s->unkBD4;
    if (s->unk20C & 4)
        a->unk24 = s->unkA9C.unk24;
    Model_Animate(a);
    Model_Draw(a);
    a = &s->unkD0C;
    Model_Animate(a);
    Model_Draw(a);
    switch (gBossStatus.unk4) {
    case 9:
    case 10:
        {
            u32 v = 2;
            NNS_G3dGeBufferOP_N(0x10, &v, 1);
        }
        {
            u32 v = 0x1a;
            NNS_G3dGeBufferOP_N(0x14, &v, 1);
        }
        NNS_G3dGetCurrentMtx(s->unk1378, s->unk13A8);
        break;
    }
    s->unk130C += s->unk1308;
    if (s->unk130C > s->unk1310)
        s->unk130C = s->unk1310;
    else if (s->unk130C < s->unk1314)
        s->unk130C = s->unk1314;
    s->unk1318 += (u16)(s->unk130C >> 12);
    a = &s->unkE44;
    MTX_RotZ33_(&a->unk24, data_0206e470[((s->unk1318 & 0xffff) >> 4) * 2], data_0206e470[((s->unk1318 & 0xffff) >> 4) * 2 + 1]);
    Model_Animate(a);
    Model_Draw(a);
    {
        u32 v = 2;
        NNS_G3dGeBufferOP_N(0x10, &v, 1);
    }
    {
        u32 v = 0x1c;
        NNS_G3dGeBufferOP_N(0x14, &v, 1);
    }
    NNS_G3dGetCurrentMtx(s->unk13CC, s->unk13FC);
    switch (gBossStatus.unk4) {
    case 7:
    case 8:
        func_ov000_022ce474(s);
        break;
    case 9:
        func_ov000_022ce194(s);
        break;
    case 10:
        func_ov000_022cde2c(s);
        break;
    }
    if ((s->unk20C & 2) || (s->unk1304 != 0 && s->unk1304 != 0x8000)) {
        if (gBossStatus.unk4 != 10)
            return;
    }
    func_ov000_022c5e1c(0x1152f0, 0x6cd988, s->unk13F0, s->unk13F8, &out);
    x = out >> 12;
    y = -(s->unk13F4 >> 12);
    if (x < 0x366)
        xb = x + 0x6cd;
    else
        xb = x - 0x6cd;
    h1 = &s->unkF84[2];
    h2 = &s->unk113C[2];
    if (!(h1->unkA & 4))
        return;
    h1->unk0 = x;
    h2->unk0 = xb;
    h1->unk4 = h2->unk4 = y;
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
