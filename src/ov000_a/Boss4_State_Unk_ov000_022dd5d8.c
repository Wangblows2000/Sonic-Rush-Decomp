#include <nitro/types.h>

typedef struct UnkStruct_022dd5d8_w {
    u8 pad[0x54];
    u32 unk54;
    u8 pad1[0x224 - 0x58];
    s32 unk224;
    u8 pad2[0xbb8 - 0x228];
    s32 unkBB8;
    s32 unkBBC;
} UnkStruct_022dd5d8_w;

typedef struct UnkStruct_022dd5d8 UnkStruct_022dd5d8;
struct UnkStruct_022dd5d8 {
    u8 pad[0x68];
    void (*func)(UnkStruct_022dd5d8 *t);
};

typedef struct UnkStruct_022dd5d8_c {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    u8 pad[0x30 - 0xc];
    u16 unk30;
    u8 pad1[0x3c - 0x32];
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
    u8 pad3[0x78 - 0x6e];
    s32 unk78;
    u8 pad4[0x80 - 0x7c];
    u16 unk80;
    u8 pad5[0x84 - 0x82];
    s32 unk84;
    u8 pad6[0x8c - 0x88];
    u16 unk8C;
    u16 unk8E;
    u8 pad7[0x92 - 0x90];
    u16 unk92;
} UnkStruct_022dd5d8_c;

typedef struct UnkStruct_022dd5d8_p {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    u8 pad1[0x17c - 0x18];
    s32 unk17C;
    s32 unk180;
    s32 unk184;
} UnkStruct_022dd5d8_p;

typedef struct UnkStruct_022dd5d8_d {
    u8 pad[0xdc];
    s32 unkDC;
} UnkStruct_022dd5d8_d;

extern UnkStruct_022dd5d8_w *gBoss4;
extern UnkStruct_022dd5d8_c data_ov000_0236a48c;
extern UnkStruct_022dd5d8_p *gPlayers;
extern UnkStruct_022dd5d8_d data_ov000_0236a444;

void Player_Action_Blank(UnkStruct_022dd5d8_p *p);
void Player_SetAnimation(UnkStruct_022dd5d8_p *p, s32 a);
void Boss4_Unk_ov000_022de65c(void);
void StageObj_SetPause(void);
void Boss4_State_Unk_ov000_022dd590(UnkStruct_022dd5d8 *t);

void Boss4_State_Unk_ov000_022dd5d8(UnkStruct_022dd5d8 *t) {
    UnkStruct_022dd5d8_w *w = gBoss4;
    UnkStruct_022dd5d8_c *c;
    w->unk224++;
    if (w->unk224 == 1) {
        Player_Action_Blank(gPlayers);
        w->unkBB8 = 0x7200;
        w->unkBBC = 0xd80;
        gPlayers->unk10 = 0x1c200;
        gPlayers->unk14 = 0x1a100;
        Player_SetAnimation(gPlayers, 0x30);
        return;
    }
    c = (UnkStruct_022dd5d8_c *)(u32)&data_ov000_0236a48c;
    Boss4_Unk_ov000_022de65c();
    data_ov000_0236a444.unkDC = 2;
    c->unk30 = 0x1000;
    c->unk44 = 0;
    c->unk40 = c->unk44;
    c->unk3C = c->unk40;
    c->unk8 = 0;
    c->unk4 = c->unk8;
    c->unk0 = c->unk4;
    c->unk50 = 0;
    c->unk4C = c->unk50;
    c->unk48 = c->unk4C;
    c->unk54 = gPlayers->unk17C;
    c->unk58 = gPlayers->unk180;
    c->unk5C = gPlayers->unk184;
    c->unk6C = 0x1000;
    c->unk80 = 0x1000;
    c->unk8C = 0x1000;
    c->unk92 = 0x1000;
    c->unk78 = 0x50000;
    c->unk84 = 0x20000;
    c->unk8E = 0x3000;
    StageObj_SetPause();
    w->unk54 |= 0x20;
    t->func = Boss4_State_Unk_ov000_022dd590;
    w->unk224 = 0;
}
