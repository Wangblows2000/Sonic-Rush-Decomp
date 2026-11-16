#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x320 - 0x20C];
    s32 unk320;
    u8 pad324[0x440 - 0x324];
    void *unk440;
} UnkStruct_022ec954_B2;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x214 - 0x20C];
    UnkStruct_022ec954_B2 *unk214;
    u8 pad218[4];
    UnkStruct_022ec954_B2 *unk21C;
    u8 pad220[0x230 - 0x220];
    u16 unk230;
    u8 pad232[0x23C - 0x232];
    void *unk23C;
    u8 pad240[0x248 - 0x240];
    void *unk248;
    u8 pad24C[0x360 - 0x24C];
    s32 unk360;
} UnkStruct_022ec954;

typedef struct {
    u8 pad0[0x30];
    s16 unk30;
    u8 pad32[0x3C - 0x32];
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    s32 unk54;
    s32 unk58;
    s32 unk5C;
    u8 pad60[0x6C - 0x60];
    s16 unk6C;
    u8 pad6E[0x78 - 0x6E];
    s32 unk78;
    u8 pad7C[0x80 - 0x7C];
    s16 unk80;
    u8 pad82[2];
    s32 unk84;
    u8 pad88[0x8C - 0x88];
    s16 unk8C;
    u16 unk8E;
    u8 pad90[0x92 - 0x90];
    s16 unk92;
} UnkStruct_0236a48c;

typedef struct {
    u8 pad0[0xDC];
    s32 unkDC;
} UnkStruct_0236a444;

typedef struct {
    u8 pad0[0x48];
    u16 unk48;
} UnkStruct_0208922c;

extern UnkStruct_StageObj *gPlayers;
extern UnkStruct_0236a444 data_ov000_0236a444;
extern UnkStruct_0236a48c data_ov000_0236a48c;
extern UnkStruct_0208922c gDisplayMain;

void BossEffect_CreateStrikeFin(s32 a, s32 x, s32 y, s32 z);
void StageObj_ClearPause(void);
void Player_SetAnimation(UnkStruct_StageObj *p, s32 a);
void Boss7_State_Unk_ov000_022ebe64(void);
void func_ov000_022e726c(void);
void Boss7_State_Unk_ov000_022ec7b4(void);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
u8 *func_02025ecc(void *p);
void func_02025e1c(void *p, s32 a, s32 b);
void func_020537b4(s32 a);
void func_020533e0(s32 a, s32 b);

static inline u32 GetVisiblePlane(void) {
    return (*(vu32 *)0x4000000 & 0x1f00) >> 8;
}

static inline void SetVisiblePlane(u32 p) {
    *(vu32 *)0x4000000 = (*(vu32 *)0x4000000 & ~0x1f00) | (p << 8);
}

void func_ov000_022ec954(UnkStruct_022ec954 *w) {
    UnkStruct_022ec954_B2 *b2 = w->unk214;
    UnkStruct_StageObj *pl = gPlayers;
    UnkStruct_0236a48c *st;
    u8 *res;

    BossEffect_CreateStrikeFin(1, (b2->base.unk10 + pl->unk10) >> 1, (b2->base.unk14 + pl->unk14) >> 1,
                        ((b2->base.unk18 + pl->unk18) >> 1) + 0x1000);
    data_ov000_0236a444.unkDC = 2;
    st = (UnkStruct_0236a48c *)(u32)&data_ov000_0236a48c;
    st->unk30 = 0x1000;
    st->unk3C = st->unk40 = st->unk44 = 0;
    st->unk48 = st->unk4C = st->unk50 = 0;
    st->unk54 = b2->base.unk10 << 4;
    st->unk58 = 0x14000 - (b2->base.unk14 << 4);
    st->unk5C = b2->base.unk18 << 4;
    st->unk6C = 0x1000;
    st->unk78 = 0x64000;
    st->unk80 = 0x1000;
    st->unk84 = -0x10000;
    st->unk8C = 0x1000;
    st->unk8E = 0xe000;
    st->unk92 = 0x1000;
    SetVisiblePlane(GetVisiblePlane() & ~4);
    w->base.unk54 &= ~0x20;
    w->unk214->base.unk54 &= ~0x20;
    w->unk21C->base.unk54 &= ~0x20;
    w->unk360 = 0x1000;
    w->unk214->unk320 = 0x1000;
    w->unk21C->unk320 = 0x1000;
    StageObj_ClearPause();
    pl->unk2E = 0x200;
    Player_SetAnimation(pl, 5);
    pl->unk5C &= ~4;
    w->unk21C->base.unk68 = Boss7_State_Unk_ov000_022ebe64;
    w->unk21C = NULL;
    w->unk214->unk440 = func_ov000_022e726c;
    w->unk214->base.unk5C &= ~0x10;
    w->unk214->base.unk58 |= 0x80;
    Sound_PlaySeqArc(0, 0, 0x9b);
    res = func_02025ecc(w->unk23C);
    func_02025e1c(res + *(u32 *)(res + *(u16 *)(res + 0xe) + 0xc), 1, 0x40);
    res = func_02025ecc(w->unk23C);
    func_02025e1c(res + *(u32 *)(res + *(u16 *)(res + 0xe) + 0xc), 1, 0x80);
    res = func_02025ecc(w->unk248);
    func_02025e1c(res + *(u32 *)(res + *(u16 *)(res + 0xe) + 0xc), 1, 0x40);
    res = func_02025ecc(w->unk248);
    func_02025e1c(res + *(u32 *)(res + *(u16 *)(res + 0xe) + 0xc), 1, 0x80);
    gDisplayMain.unk48 = 0x10;
    func_020537b4(0xd);
    func_020533e0(0x13, 0xf0);
    w->unk230 = 0;
    w->base.unk68 = Boss7_State_Unk_ov000_022ec7b4;
}
