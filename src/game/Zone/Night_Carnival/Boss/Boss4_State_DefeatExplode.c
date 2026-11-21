#include <nitro/types.h>

typedef struct UnkStruct_022dd1e8_a {
    u8 pad[4];
    u32 unk4;
    u8 pad1[0x48 - 8];
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    u8 pad2[0x90 - 0x54];
    u8 unk90[0x118 - 0x90];
    s32 unk118;
    u8 pad3[0x138 - 0x11c];
} UnkStruct_022dd1e8_a;

typedef struct UnkStruct_022dd1e8_m {
    u8 m[0x24];
    s32 x;
    s32 y;
    s32 z;
} UnkStruct_022dd1e8_m;

typedef struct UnkStruct_022dd1e8_w {
    u8 pad[0x54];
    u32 unk54;
    u8 pad0[0x224 - 0x58];
    s32 unk224;
    UnkStruct_022dd1e8_a unk228[7];
    void *unkAB0;
    u8 pad2[0xaf8 - 0xab4];
    UnkStruct_022dd1e8_m unkAF8;
    UnkStruct_022dd1e8_m unkB28;
    u8 pad3[0xbb8 - 0xb58];
    s32 unkBB8;
    s32 unkBBC;
} UnkStruct_022dd1e8_w;

typedef struct UnkStruct_022dd1e8 UnkStruct_022dd1e8;
struct UnkStruct_022dd1e8 {
    u8 pad[0x68];
    void (*func)(UnkStruct_022dd1e8 *t);
};

typedef struct UnkStruct_022dd1e8_c {
    u8 pad[0x54];
    s32 unk54;
    s32 unk58;
    s32 unk5C;
    u8 pad1[0x78 - 0x60];
    s32 unk78;
    u8 pad2[0x84 - 0x7c];
    s32 unk84;
    u8 pad3[0x8e - 0x88];
    u16 unk8E;
} UnkStruct_022dd1e8_c;

typedef struct UnkStruct_022dd1e8_p {
    u8 pad[0x5c];
    u16 unk5C;
    u8 pad1[0x32c - 0x5e];
    u32 unk32C;
} UnkStruct_022dd1e8_p;

typedef struct UnkStruct_022dd1e8_q {
    u8 pad[0x248];
    s32 unk248;
} UnkStruct_022dd1e8_q;

typedef struct UnkStruct_022dd1e8_g {
    u8 pad[0x48];
    s16 unk48;
} UnkStruct_022dd1e8_g;

extern UnkStruct_022dd1e8_w *gBoss4;
extern UnkStruct_022dd1e8_c data_ov000_0236a48c;
extern UnkStruct_022dd1e8_p *gPlayers;
extern UnkStruct_022dd1e8_g gDisplayMain;

void Model_SetAnimation(void *p, s32 a, void *b, s32 c, s32 d);
void NNS_G3dRenderObjResetCallBack(void *p);
void MTX_Identity33_(void *m);
void StageObj_ClearPause(void);
UnkStruct_022dd1e8_q *BossEffect_CreateBlast4(s32 a, s32 b, s32 c, s32 d);
void NaviMsg_SetFaceAnim(s32 a);
void NaviMsg_ShowMessage(s32 a, s32 b);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void Boss4_State_DefeatFinish(UnkStruct_022dd1e8 *t);

static inline u32 GX_GetVisiblePlane(void) {
    return (*(vu32 *)0x4000000 & 0x1f00) >> 8;
}

static inline void GX_SetVisiblePlane(u32 p) {
    *(vu32 *)0x4000000 = (*(vu32 *)0x4000000 & ~0x1f00) | (p << 8);
}

void Boss4_State_DefeatExplode(UnkStruct_022dd1e8 *t) {
    UnkStruct_022dd1e8_w *w = gBoss4;
    UnkStruct_022dd1e8_a *a;
    s32 i;
    UnkStruct_022dd1e8_c *c = (UnkStruct_022dd1e8_c *)(u32)&data_ov000_0236a48c;
    c->unk78 = 0x1f0fb8;
    c->unk84 = 0x64000;
    c->unk8E = 0;
    c->unk54 = 0;
    c->unk58 = -0x1d8000;
    c->unk5C = 0;
    w->unkBB8 = 0;
    w->unkBBC = 0;
    a = &w->unk228[2];
    Model_SetAnimation(a, 0, w->unkAB0, 1, 0);
    NNS_G3dRenderObjResetCallBack(a->unk90);
    a = &w->unk228[1];
    Model_SetAnimation(a, 0, w->unkAB0, 3, 0);
    NNS_G3dRenderObjResetCallBack(a->unk90);
    a = &w->unk228[4];
    Model_SetAnimation(a, 0, w->unkAB0, 2, 0);
    a->unk118 = 0x1000;
    NNS_G3dRenderObjResetCallBack(a->unk90);
    a->unk4C = -0x200000;
    MTX_Identity33_(&w->unkAF8);
    w->unkAF8.x = 0;
    w->unkAF8.y = -0x200000;
    w->unkAF8.z = 0;
    MTX_Identity33_(&w->unkB28);
    w->unkB28.x = 0;
    w->unkB28.y = -0x200000;
    w->unkB28.z = 0;
    GX_SetVisiblePlane(GX_GetVisiblePlane() & ~4);
    w->unk54 &= ~0x20;
    StageObj_ClearPause();
    gPlayers->unk5C |= 0x20;
    gPlayers->unk32C |= 0x200000;
    BossEffect_CreateBlast4(0, 0, 0x1ce00, 0xfa00)->unk248 = 0x800;
    gDisplayMain.unk48 = 0x10;
    NaviMsg_SetFaceAnim(0xd);
    NaviMsg_ShowMessage(0x13, 0xf0);
    Sound_PlaySeqArc(0, 0, 0x7d);
    for (i = 0; i < 7; i++)
        w->unk228[i].unk4 &= ~2;
    t->func = Boss4_State_DefeatFinish;
    w->unk224 = 0;
}
