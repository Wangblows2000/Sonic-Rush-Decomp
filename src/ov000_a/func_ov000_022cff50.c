#include <nitro/types.h>

typedef struct UnkStruct_022cff50_a {
    u8 pad[0xe4];
    s32 *unkE4;
    u8 pad1[0x114 - 0xe8];
    s32 unk114;
    u8 pad2[0x138 - 0x118];
} UnkStruct_022cff50_a;

typedef struct UnkStruct_022cff50_h {
    u8 pad[0xa];
    u16 unkA;
    u8 pad1[0x2c - 0xc];
} UnkStruct_022cff50_h;

typedef struct UnkStruct_022cff50 UnkStruct_022cff50;
struct UnkStruct_022cff50 {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 pad0[0x54 - 0x1c];
    u32 unk54;
    u8 pad0b[0x68 - 0x58];
    void (*func)(UnkStruct_022cff50 *s);
    u8 pad1[0x20c - 0x6c];
    u32 unk20C;
    u8 pad1b[0x460 - 0x210];
    s32 unk460;
    u8 pad1c[0x598 - 0x464];
    s32 unk598;
    u8 pad2[0xa9c - 0x59c];
    UnkStruct_022cff50_a unkA9C;
    UnkStruct_022cff50_a unkBD4;
    UnkStruct_022cff50_a unkD0C;
    u8 pad3[0xf7c - 0xe44];
    s32 unkF7C;
    u8 pad3b[0xf84 - 0xf80];
    UnkStruct_022cff50_h unkF84[10];
    u8 pad4[0x12f4 - 0x113c];
    s32 unk12F4;
    u8 pad5[0x1304 - 0x12f8];
    u16 unk1304;
    u8 pad5b[0x1308 - 0x1306];
    s32 unk1308;
    s32 unk130C;
    s32 unk1310;
    s32 unk1314;
    u16 unk1318;
    u8 pad6[0x1444 - 0x131a];
    u16 *unk1444;
    u8 pad6b[0x145c - 0x1448];
    s32 unk145C;
    s32 unk1460;
};

typedef struct UnkStruct_022cff50_c {
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
    s32 unk60;
    s32 unk64;
    s32 unk68;
    u16 unk6C;
    u8 pad1[0x80 - 0x6e];
    u16 unk80;
    u8 pad2[0x8c - 0x82];
    u16 unk8C;
    u8 pad3[0x92 - 0x8e];
    u16 unk92;
} UnkStruct_022cff50_c;

typedef struct UnkStruct_022cff50_pl {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 pad1[0x5c - 0x1c];
    u16 unk5C;
    u8 pad2[0x3b2 - 0x5e];
    u16 unk3B2;
} UnkStruct_022cff50_pl;

typedef struct UnkStruct_022cff50_5 {
    u8 pad[8];
    u16 unk8;
    u16 unkA;
    u8 pad1[0x48 - 0xc];
    s16 unk48;
} UnkStruct_022cff50_5;

typedef struct UnkStruct_022cff50_4 {
    u8 pad[0x18];
    u32 unk18;
} UnkStruct_022cff50_4;

typedef struct UnkStruct_022cff50_7 {
    u8 pad[4];
    s32 unk4;
} UnkStruct_022cff50_7;

typedef struct UnkStruct_022cff50_cam {
    u8 pad[0xdc];
    s32 unkDC;
} UnkStruct_022cff50_cam;

typedef struct UnkStruct_022cff50_8 {
    u8 pad[0x24];
    s32 unk24;
} UnkStruct_022cff50_8;

extern UnkStruct_022cff50 *data_ov000_02369ae0;
extern UnkStruct_022cff50_c data_ov000_0236a48c;
extern UnkStruct_022cff50_4 data_022c4560;
extern UnkStruct_022cff50_7 data_ov000_02369ac0;
extern UnkStruct_022cff50_pl *gPlayers;
extern UnkStruct_022cff50_5 gDisplayMain;
extern UnkStruct_022cff50_cam data_ov000_0236a444;
extern UnkStruct_022cff50_8 data_ov000_02369ae8;
extern s32 data_ov000_023651f8[][3];
extern s32 data_ov000_023651fc[][3];
extern s32 data_ov000_02365200[][3];
extern u32 data_ov000_0236510c[];
extern u16 data_ov000_02362dcc[];

void func_ov000_022cfb90(UnkStruct_022cff50 *s);
void func_0201bfbc(s32 a);
void StageObj_SetPause(void);
void Model_SetAnimation(void *p, s32 a, s32 b, s32 c, s32 d);
void func_ov000_02347c44(UnkStruct_022cff50_pl *p);
void Player_SetAnimation(UnkStruct_022cff50_pl *p, s32 a);
void ScreenShake_Start(s32 a);
u32 func_ov000_0235b0a8(void);
void func_ov000_0235b08c(u32 a);
void func_ov000_022cfe5c(s32 a, s32 b);
void func_ov000_022c5e80(s32 len, s32 b, s32 a, s32 *outX, s32 *outY, u16 *outAng);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void func_ov000_022c5d4c(s32 a, u32 *p1, u16 *p2, u16 *p3, u16 n);

void func_ov000_022cff50(void) {
    UnkStruct_022cff50_a *x;
    UnkStruct_022cff50_a *y;
    UnkStruct_022cff50 *s = data_ov000_02369ae0;
    s32 i;
    u32 v;
    UnkStruct_022cff50_a *z;
    UnkStruct_022cff50_c *c;
    s->func = func_ov000_022cfb90;
    x = &s->unkA9C;
    y = &s->unkBD4;
    z = &s->unkD0C;
    c = (UnkStruct_022cff50_c *)(u32)&data_ov000_0236a48c;
    func_0201bfbc(0);
    StageObj_SetPause();
    s->unk54 |= 0x20;
    data_022c4560.unk18 &= ~1;
    s->unk460 = s->unk598 = 0;
    s->unk1308 = 0;
    s->unk130C = 0;
    s->unk1310 = 0;
    s->unk1314 = 0;
    s->unk1318 = 0;
    if (data_ov000_02369ac0.unk4 == 9) {
        Model_SetAnimation(x, 0, s->unkF7C, 0xe, 0);
        Model_SetAnimation(y, 0, s->unkF7C, 0x2e, 0);
        Model_SetAnimation(z, 0, s->unkF7C, 0x4a, 0);
        *z->unkE4 = 0;
        *y->unkE4 = *z->unkE4;
        *x->unkE4 = *y->unkE4;
        s->unk1460 = 0;
        s->unk1304 = 0;
    } else {
        if (*(u16 *)&s->unk145C) {
            Model_SetAnimation(x, 0, s->unkF7C, 0x1a, 0);
            Model_SetAnimation(y, 0, s->unkF7C, 0x37, 0);
            Model_SetAnimation(z, 0, s->unkF7C, 0x56, 0);
            s->unk1460 = 2;
            *z->unkE4 = 0x9a000;
            *y->unkE4 = *z->unkE4;
            *x->unkE4 = *y->unkE4;
        } else {
            Model_SetAnimation(x, 0, s->unkF7C, 0x14, 0);
            Model_SetAnimation(y, 0, s->unkF7C, 0x33, 0);
            Model_SetAnimation(z, 0, s->unkF7C, 0x50, 0);
            s->unk1460 = 1;
            *z->unkE4 = 0x9a000;
            *y->unkE4 = *z->unkE4;
            *x->unkE4 = *y->unkE4;
        }
        s->unk1304 = 0x4000;
    }
    s->unk10 = 0;
    s->unk14 = 0x20000;
    s->unk18 = 0;
    x->unk114 = 0;
    y->unk114 = 0;
    z->unk114 = 0;
    gPlayers->unk10 = data_ov000_023651f8[s->unk1460][0] << 8;
    gPlayers->unk14 = data_ov000_023651fc[s->unk1460][0] << 8;
    gPlayers->unk18 = data_ov000_02365200[s->unk1460][0] << 8;
    func_ov000_02347c44(gPlayers);
    gPlayers->unk3B2 = 0;
    gPlayers->unk5C &= ~0x20;
    Player_SetAnimation(gPlayers, 0x30);
    for (i = 0; i < 10; i++) {
        s->unkF84[i].unkA &= ~4;
        s->unkF84[i].unkA |= 0x1000;
    }
    data_ov000_02369ac0.unk4 = 0xc;
    ScreenShake_Start(0);
    func_ov000_0235b08c(func_ov000_0235b0a8() | 1);
    s->unk12F4 = 0x1e;
    s->unk145C = 0;
    gDisplayMain.unk48 = 0x10;
    *(vu16 *)0x400000c = (*(vu16 *)0x400000c & 0x43) | 0x700;
    *(vu32 *)0x4000018 = 0;
    gDisplayMain.unk8 = 0;
    gDisplayMain.unkA = 0;
    func_ov000_022cfe5c(s->unk1460, 0);
    v = (*(vu32 *)0x4000000 & 0x1f00) >> 8;
    *(vu32 *)0x4000000 = (*(vu32 *)0x4000000 & ~0x1f00) | ((v | 4) << 8);
    data_ov000_0236a444.unkDC = 2;
    s->unk20C &= ~0x20;
    c->unk3C = c->unk40 = c->unk44 = 0;
    c->unk48 = c->unk4C = c->unk50 = 0;
    func_ov000_022c5e80(0x1152f0, 0x6cd988, data_ov000_023651f8[s->unk1460][0] << 12, &c->unk60, &c->unk68, NULL);
    c->unk54 = c->unk60;
    c->unk58 = c->unk64 = -data_ov000_023651fc[s->unk1460][0] << 12;
    c->unk5C = c->unk68;
    c->unk6C = 0x1000;
    c->unk80 = 0x1000;
    c->unk8C = 0x1000;
    c->unk92 = 0x1000;
    c->unk30 = 0x1000;
    Sound_PlaySeqArc(0, 0, 0x7c);
    func_ov000_022c5d4c(data_ov000_02369ae8.unk24, data_ov000_0236510c, data_ov000_02369ae0->unk1444, data_ov000_02362dcc, 1);
}
