#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x20];
    union {
        u16 raw;
        struct {
            u16 b0 : 1;
            u16 b1 : 5;
            u16 b6 : 2;
            u16 b8 : 8;
        } bits;
    } unk20;
    u8 pad22[2];
    struct {
        u16 b0 : 5;
        u16 b5 : 11;
    } unk24;
} UnkStruct_0208922c;

typedef struct {
    u8 pad0[0x14C];
    s32 unk14C;
    s32 unk150;
    s32 unk154;
    u8 pad158[0x248 - 0x158];
    s32 unk248;
} UnkStruct_Fx;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x3D0 - 0x20C];
    s32 unk3D0;
    u8 pad3D4[0x440 - 0x3D4];
    void *unk440;
} UnkStruct_022e899c;

extern UnkStruct_0208922c gDisplayMain;
extern UnkStruct_StageObj *gPlayers;

void Boss7_SetRivalAnimation(UnkStruct_022e899c *w, s32 a);
void Boss7_SetCollidersEnabled(UnkStruct_022e899c *w, s32 a, s32 b);
s32 FX_Atan2(s32 y, s32 x);
UnkStruct_Fx *BossEffect_CreateHoming(s32 a, s32 x, s32 y, s32 z);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
UnkStruct_StageObj *EffBoostA2_3D_Create(UnkStruct_022e899c *w, s32 a, s32 b, s32 c, s32 d);
void ScreenShake_Start(s32 a);
void Boss7PartD5_MoveState_AirDash(UnkStruct_022e899c *w);

static inline s32 FX_Mul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

void Boss7PartD5_MoveState_StartAirDash(UnkStruct_022e899c *w) {
    s32 a;
    UnkStruct_Fx *fx;
    UnkStruct_StageObj *o;

    Boss7_SetRivalAnimation(w, 0x31);
    Boss7_SetCollidersEnabled(w, 0, 1);
    a = FX_Atan2(0x1f000 - w->base.unk14, gPlayers->unk10 - w->base.unk10);
    w->base.unk8 = a;
    if (w->base.unk10 > gPlayers->unk10)
        w->base.unk5C |= 1;
    else
        w->base.unk5C &= ~1;
    if (w->unk3D0 == 0) {
        fx = BossEffect_CreateHoming(1, w->base.unk10, w->base.unk14, w->base.unk18 + 8);
        fx->unk248 = 0x999;
        fx->unk14C = FX_Mul(fx->unk14C, 0x1800);
        fx->unk150 = FX_Mul(fx->unk150, 0x1800);
        fx->unk154 = FX_Mul(fx->unk154, 0x1800);
        Sound_PlaySeqArc(0, 0, 7);
        o = EffBoostA2_3D_Create(w, 0, 0, 0, 0);
    } else {
        Sound_PlaySeqArc(0, 0, 0xbd);
        o = EffBoostA2_3D_Create(w, 1, 0, 0, 0);
    }
    if (o != NULL) {
        if (o->unk5C & 1)
            o->unk1 = (a + 0x8000) >> 8;
        else
            o->unk1 = a >> 8;
    }
    gDisplayMain.unk20.raw = 0;
    gDisplayMain.unk20.bits.b0 = 1;
    gDisplayMain.unk20.bits.b6 = 2;
    gDisplayMain.unk24.b0 = 0xb;
    ScreenShake_Start(4);
    w->unk440 = Boss7PartD5_MoveState_AirDash;
    ((void (*)(UnkStruct_022e899c *))w->unk440)(w);
}
