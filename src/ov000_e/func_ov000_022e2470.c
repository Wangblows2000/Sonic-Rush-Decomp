#include <nitro/types.h>

typedef struct UnkStruct_022e2470_a {
    u8 pad[0xe4];
    s32 *unkE4;
    u8 pad1[0x10c - 0xe8];
    u8 unk10C;
    u8 pad1b[0x114 - 0x10d];
    s32 unk114;
} UnkStruct_022e2470_a;

typedef struct UnkStruct_022e2470_h {
    u8 pad[0xa];
    u16 unkA;
    u8 pad1[0x2c - 0xc];
} UnkStruct_022e2470_h;

typedef struct UnkStruct_022e2470 UnkStruct_022e2470;
struct UnkStruct_022e2470 {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 pad0[0x54 - 0x1c];
    u32 unk54;
    u8 pad0b[0x68 - 0x58];
    void (*func)(UnkStruct_022e2470 *s);
    u8 pad1[0x20c - 0x6c];
    u32 unk20C;
    u8 pad2[0x960 - 0x210];
    UnkStruct_022e2470_a unk960;
    u8 pad3[0xd08 - 0x960 - sizeof(UnkStruct_022e2470_a)];
    s32 unkD08;
    u8 pad3b[0xd5e - 0xd0c];
    u16 unkD5E;
    UnkStruct_022e2470_h unkD60[11];
    u8 pad4[0xf4c - 0xd60 - 11 * 0x2c];
    u16 unkF4C;
    u8 pad4b[0xf50 - 0xf4e];
    s32 unkF50;
    u8 pad5[0x110c - 0xf54];
    s32 unk110C;
    u8 pad5b[0x1114 - 0x1110];
    s32 unk1114;
    s32 unk1118;
    s32 unk111C;
};

typedef struct UnkStruct_022e2470_c {
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
} UnkStruct_022e2470_c;

typedef struct UnkStruct_022e2470_pl {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 pad1[0x5c - 0x1c];
    u16 unk5C;
    u8 pad2[0x3b2 - 0x5e];
    u16 unk3B2;
} UnkStruct_022e2470_pl;

typedef struct UnkStruct_022e2470_5 {
    u8 pad[8];
    u16 unk8;
    u16 unkA;
    u8 pad1[0x48 - 0xc];
    s16 unk48;
} UnkStruct_022e2470_5;

typedef struct UnkStruct_022e2470_4 {
    u8 pad[0x18];
    u32 unk18;
} UnkStruct_022e2470_4;

typedef struct UnkStruct_022e2470_7 {
    u8 pad[4];
    s32 unk4;
} UnkStruct_022e2470_7;

typedef struct UnkStruct_022e2470_cam {
    u8 pad[0xdc];
    s32 unkDC;
} UnkStruct_022e2470_cam;

typedef struct UnkStruct_022e2470_8 {
    u8 pad[0x18];
    s32 unk18;
} UnkStruct_022e2470_8;

extern UnkStruct_022e2470 *data_ov000_02369b4c;
extern UnkStruct_022e2470_c data_ov000_0236a48c;
extern UnkStruct_022e2470_4 data_022c4560;
extern UnkStruct_022e2470_7 data_ov000_02369ac0;
extern UnkStruct_022e2470_pl *gPlayers;
extern UnkStruct_022e2470_5 gDisplayMain;
extern UnkStruct_022e2470_cam data_ov000_0236a444;
extern UnkStruct_022e2470_8 data_ov000_02369b50;
extern s32 data_ov000_023658cc[][3];
extern s32 data_ov000_023658d0[][3];
extern s32 data_ov000_023658d4[][3];
extern u32 data_ov000_023657e0[];
extern u16 data_ov000_023636f4[];

void Boss6_State_Unk_ov000_022e213c(UnkStruct_022e2470 *s);
void NNS_SndPlayerStopSeqAll(s32 a);
void StageObj_SetPause(void);
void Model_SetAnimation(void *p, s32 a, s32 b, s32 c, s32 d);
void func_ov000_02347c44(UnkStruct_022e2470_pl *p);
void Player_SetAnimation(UnkStruct_022e2470_pl *p, s32 a);
void ScreenShake_Start(s32 a);
u32 func_ov000_0235b0a8(void);
void func_ov000_0235b08c(u32 a);
void func_ov000_022e237c(s32 a, s32 b);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void func_ov000_022c5d4c(s32 a, u32 *p1, s32 p2, u16 *p3, u16 n);

void func_ov000_022e2470(void) {
    UnkStruct_022e2470 *s = data_ov000_02369b4c;
    UnkStruct_022e2470_a *x;
    UnkStruct_022e2470_c *c;
    s32 i;
    u32 v;
    s->func = Boss6_State_Unk_ov000_022e213c;
    x = &s->unk960;
    c = (UnkStruct_022e2470_c *)(u32)&data_ov000_0236a48c;
    NNS_SndPlayerStopSeqAll(0);
    StageObj_SetPause();
    x->unk114 = 0;
    s->unk54 |= 0x20;
    data_022c4560.unk18 &= ~1;
    if (data_ov000_02369ac0.unk4 == 0x1f) {
        Model_SetAnimation(x, 0, s->unkD08, 4, 0);
        x->unk10C &= ~4;
        x->unk114 = 0;
        *x->unkE4 = 0x28000;
        s->unk111C = 0;
    } else {
        Model_SetAnimation(x, 0, s->unkD08, 0x14, 0);
        x->unk10C &= ~4;
        x->unk114 = 0;
        *x->unkE4 = 0x12000;
        s->unk111C = 1;
    }
    s->unk10 = 0x1a000;
    s->unk14 = 0x20000;
    s->unk18 = 0;
    gPlayers->unk10 = data_ov000_023658cc[s->unk111C][0] << 8;
    gPlayers->unk14 = data_ov000_023658d0[s->unk111C][0] << 8;
    gPlayers->unk18 = data_ov000_023658d4[s->unk111C][0] << 8;
    s->unkF4C = 0;
    func_ov000_02347c44(gPlayers);
    gPlayers->unk3B2 = 0;
    gPlayers->unk5C &= ~0x20;
    Player_SetAnimation(gPlayers, 0x30);
    s->unk20C &= ~0x14;
    for (i = 0; i < 11; i++) {
        s->unkD60[i].unkA &= ~4;
        s->unkD60[i].unkA |= 0x1000;
    }
    s->unkD5E = 0;
    ScreenShake_Start(0);
    func_ov000_0235b08c(func_ov000_0235b0a8() | 1);
    data_ov000_02369ac0.unk4 = 0x23;
    s->unkF50 = 0x1e;
    s->unk1118 = 0;
    gDisplayMain.unk48 = 0x10;
    *(vu16 *)0x400000c = (*(vu16 *)0x400000c & 0x43) | 0x700;
    *(vu32 *)0x4000018 = 0;
    gDisplayMain.unk8 = 0;
    gDisplayMain.unkA = 0;
    func_ov000_022e237c(s->unk111C, 0);
    v = (*(vu32 *)0x4000000 & 0x1f00) >> 8;
    *(vu32 *)0x4000000 = (*(vu32 *)0x4000000 & ~0x1f00) | ((v | 4) << 8);
    data_ov000_0236a444.unkDC = 2;
    c->unk3C = c->unk40 = c->unk44 = 0;
    c->unk48 = c->unk4C = c->unk50 = 0;
    c->unk54 = c->unk60 = data_ov000_023658cc[s->unk111C][0] << 12;
    c->unk58 = c->unk64 = -data_ov000_023658d0[s->unk111C][0] << 12;
    c->unk5C = c->unk68 = data_ov000_023658d4[s->unk111C][0] << 12;
    c->unk6C = 0x1000;
    c->unk80 = 0x1000;
    c->unk8C = 0x1000;
    c->unk92 = 0x1000;
    c->unk30 = 0x1000;
    Sound_PlaySeqArc(0, 0, 0x7c);
    func_ov000_022c5d4c(data_ov000_02369b50.unk18, data_ov000_023657e0, data_ov000_02369b4c->unk110C, data_ov000_023636f4, 1);
    s->unk1114 = 0;
}
