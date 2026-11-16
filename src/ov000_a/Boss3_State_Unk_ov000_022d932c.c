#include <nitro/types.h>

typedef struct UnkStruct_022d932c_a {
    u8 pad[0x48];
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    u8 pad1[0x10c - 0x54];
    u8 unk10C;
    u8 pad2[0x114 - 0x10d];
    s32 unk114;
    u8 pad3[0x170 - 0x118];
} UnkStruct_022d932c_a;

typedef struct UnkStruct_022d932c_brl {
    u8 pad[0x54];
    u32 unk54;
    u8 pad1[0x2c4 - 0x58];
    UnkStruct_022d932c_a unk2C4;
    void *unk434;
} UnkStruct_022d932c_brl;

typedef struct UnkStruct_022d932c_rock {
    u8 pad[0x5c];
    u16 unk5C;
} UnkStruct_022d932c_rock;

typedef struct UnkStruct_022d932c UnkStruct_022d932c;
struct UnkStruct_022d932c {
    u8 pad[0x54];
    u32 unk54;
    u8 pad1[0x68 - 0x58];
    void (*func)(UnkStruct_022d932c *s);
    u8 pad2[0x214 - 0x6c];
    UnkStruct_022d932c_brl *unk214;
    UnkStruct_022d932c_rock *unk218;
    u8 pad3[0x224 - 0x21c];
    u16 unk224;
    u8 pad4[0x350 - 0x226];
    s32 unk350;
};

typedef struct UnkStruct_022d932c_c {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    u8 pad[0x18 - 0xc];
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    u8 pad1[0x30 - 0x24];
    u16 unk30;
    u8 pad2[0x3c - 0x32];
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    s32 unk54;
    s32 unk58;
    s32 unk5C;
    u8 pad3[0x6c - 0x60];
    u16 unk6C;
} UnkStruct_022d932c_c;

typedef struct UnkStruct_022d932c_d {
    u8 pad[0xdc];
    s32 unkDC;
} UnkStruct_022d932c_d;

typedef struct UnkStruct_022d932c_q {
    u8 pad[0x13e];
    u8 unk13E;
    u8 unk13F;
    u8 unk140;
    u8 pad1[0x248 - 0x141];
    s32 unk248;
} UnkStruct_022d932c_q;

typedef struct UnkStruct_022d932c_g {
    u8 pad[0x48];
    s16 unk48;
} UnkStruct_022d932c_g;

extern UnkStruct_022d932c_d data_ov000_0236a444;
extern UnkStruct_022d932c_c data_ov000_0236a48c;
extern UnkStruct_022d932c_g gDisplayMain;

void Model_SetAnimation(void *p, s32 a, void *b, s32 c, s32 d);
void StageObj_ClearPause(void);
UnkStruct_022d932c_q *BossEffect_CreateBlast3(s32 a, s32 b, s32 c, s32 d);
void NaviMsg_SetFaceAnim(s32 a);
void NaviMsg_ShowMessage(s32 a, s32 b);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void Boss3_State_Unk_ov000_022d9264(UnkStruct_022d932c *s);

static inline u32 GX_GetVisiblePlane(void) {
    return (*(vu32 *)0x4000000 & 0x1f00) >> 8;
}

static inline void GX_SetVisiblePlane(u32 p) {
    *(vu32 *)0x4000000 = (*(vu32 *)0x4000000 & ~0x1f00) | (p << 8);
}

void Boss3_State_Unk_ov000_022d932c(UnkStruct_022d932c *s) {
    UnkStruct_022d932c_brl *a = s->unk214;
    UnkStruct_022d932c_rock *b = s->unk218;
    UnkStruct_022d932c_c *c;
    UnkStruct_022d932c_q *q;
    Model_SetAnimation(&a->unk2C4, 0, a->unk434, 0x16, 0);
    a->unk2C4.unk10C &= ~4;
    a->unk2C4.unk114 = 0x1000;
    data_ov000_0236a444.unkDC = 1;
    c = (UnkStruct_022d932c_c *)(u32)&data_ov000_0236a48c;
    c->unk8 = 0;
    c->unk4 = c->unk8;
    c->unk0 = c->unk4;
    c->unk18 = 0;
    c->unk1C = -0x1b0000;
    c->unk20 = a->unk2C4.unk50 + 0x10e000;
    c->unk30 = 0x1000;
    c->unk44 = 0;
    c->unk40 = c->unk44;
    c->unk3C = c->unk40;
    c->unk50 = 0;
    c->unk4C = c->unk50;
    c->unk48 = c->unk4C;
    c->unk54 = a->unk2C4.unk48;
    c->unk58 = a->unk2C4.unk4C + 0x50000;
    c->unk5C = a->unk2C4.unk50;
    c->unk6C = 0x1000;
    GX_SetVisiblePlane(GX_GetVisiblePlane() & ~4);
    b->unk5C |= 0x20;
    s->unk54 &= ~0x20;
    a->unk54 &= ~0x20;
    s->unk350 = 0x1000;
    a->unk2C4.unk114 = 0x1000;
    StageObj_ClearPause();
    q = BossEffect_CreateBlast3(0, a->unk2C4.unk48 >> 4, -a->unk2C4.unk4C >> 4, a->unk2C4.unk50 >> 4);
    q->unk13E = 0x13;
    q->unk13F = 3;
    q->unk140 = 0;
    q->unk248 = 0x800;
    NaviMsg_SetFaceAnim(0xd);
    NaviMsg_ShowMessage(0x13, 0xf0);
    gDisplayMain.unk48 = 0x10;
    Sound_PlaySeqArc(0, 0, 0x7d);
    s->unk224 = 0;
    s->func = Boss3_State_Unk_ov000_022d9264;
}
