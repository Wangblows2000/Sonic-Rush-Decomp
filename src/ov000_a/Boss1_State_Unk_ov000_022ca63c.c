#include <nitro/types.h>

typedef struct UnkStruct_022ca63c_3 {
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
} UnkStruct_022ca63c_3;

typedef struct UnkStruct_022ca63c_4 {
    u8 pad[0x54];
    u32 unk54;
    u8 pad1[0x304 - 0x58];
    s32 unk304;
    u8 pad2[4];
    u8 unk30C[0x330 - 0x30c];
    u8 unk330[0x420 - 0x330];
    s32 unk420;
} UnkStruct_022ca63c_4;

typedef struct UnkStruct_022ca63c UnkStruct_022ca63c;
struct UnkStruct_022ca63c {
    u8 pad[0x54];
    u32 unk54;
    u8 pad0[0x68 - 0x58];
    void (*func)(UnkStruct_022ca63c *s);
    u8 pad1[0x20c - 0x6c];
    u32 unk20C;
    UnkStruct_022ca63c_3 *unk210;
    UnkStruct_022ca63c_4 *unk214;
    u8 pad2[0x220 - 0x218];
    u16 unk220;
    u8 pad3[0x350 - 0x222];
    s32 unk350;
};

typedef struct UnkStruct_022ca63c_cam {
    u8 pad[0xdc];
    s32 unkDC;
} UnkStruct_022ca63c_cam;

typedef struct UnkStruct_022ca63c_c {
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
} UnkStruct_022ca63c_c;

typedef struct UnkStruct_022ca63c_pl {
    u8 pad[0x5c];
    u16 unk5C;
    u8 pad1[0x32c - 0x5e];
    u32 unk32C;
    u8 pad2[0x420 - 0x330];
    u16 unk420;
    u16 unk422;
    u16 unk424;
} UnkStruct_022ca63c_pl;

typedef struct UnkStruct_022ca63c_5 {
    u8 pad[0x48];
    s16 unk48;
} UnkStruct_022ca63c_5;

extern UnkStruct_022ca63c_cam data_ov000_0236a444;
extern UnkStruct_022ca63c_c data_ov000_0236a48c;
extern UnkStruct_022ca63c_pl *gPlayers;
extern UnkStruct_022ca63c_5 gDisplayMain;

void Boss1_SetPartAnimation(void *p, s32 a, s32 b, s32 c, s32 d);
void MTX_Identity33_(void *m);
void StageObj_ClearPause(void);
BOOL Boss1_Unk_ov000_022cc6d4(void);
void BossEffect_CreateBlast5(s32 a, s32 x, s32 y, s32 z);
u8 *BossEffect_CreateBlast1(s32 a, s32 x, s32 y, s32 z);
void NaviMsg_SetFaceAnim(s32 a);
void NaviMsg_ShowMessage(s32 a, s32 b);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void Boss1_State_Unk_ov000_022ca574(UnkStruct_022ca63c *s);

void Boss1_State_Unk_ov000_022ca63c(UnkStruct_022ca63c *s) {
    u32 v;
    UnkStruct_022ca63c_c *c;
    Boss1_SetPartAnimation(s->unk210->unk228, s->unk210->unk220, 0x1f, 0x1000, 0);
    Boss1_SetPartAnimation(s->unk214->unk30C, s->unk214->unk304, 0xc, 0x1000, 0);
    s->unk20C |= 1;
    MTX_Identity33_(s->unk214->unk330);
    data_ov000_0236a444.unkDC = 2;
    c = (UnkStruct_022ca63c_c *)(u32)&data_ov000_0236a48c;
    c->unk30 = 0x1000;
    c->unk3C = c->unk40 = c->unk44 = 0;
    c->unk48 = c->unk4C = c->unk50 = 0;
    c->unk54 = 0xe1000;
    c->unk58 = -0x180000;
    c->unk5C = 0;
    c->unk6C = 0x1000;
    c->unk78 = 0x12c000;
    c->unk80 = 0x1000;
    c->unk84 = -0x80000;
    c->unk8C = 0x1000;
    c->unk8E = 0xfaab;
    c->unk92 = 0x1000;
    v = (*(vu32 *)0x4000000 & 0x1f00) >> 8;
    *(vu32 *)0x4000000 = (*(vu32 *)0x4000000 & ~0x1f00) | ((v & ~4) << 8);
    s->unk54 &= ~0x20;
    s->unk210->unk54 &= ~0x20;
    s->unk214->unk54 &= ~0x20;
    s->unk350 = 0x1000;
    s->unk210->unk33C = 0x1000;
    s->unk214->unk420 = 0x1000;
    StageObj_ClearPause();
    gPlayers->unk5C |= 0x20;
    gPlayers->unk32C |= 0x200000;
    gPlayers->unk420 = gPlayers->unk422 = gPlayers->unk424 = 0;
    if (Boss1_Unk_ov000_022cc6d4()) {
        BossEffect_CreateBlast5(0, s->unk210->unk10, s->unk210->unk14, s->unk210->unk18);
    } else {
        u8 *obj = BossEffect_CreateBlast1(0, 0xe100, 0x20000, 0);
        *(s32 *)(obj + 0x248) = 0x800;
    }
    NaviMsg_SetFaceAnim(0xd);
    NaviMsg_ShowMessage(0x13, 0xf0);
    gDisplayMain.unk48 = 0x10;
    Sound_PlaySeqArc(0, 0, 0x7d);
    s->unk220 = 0;
    s->func = Boss1_State_Unk_ov000_022ca574;
}
