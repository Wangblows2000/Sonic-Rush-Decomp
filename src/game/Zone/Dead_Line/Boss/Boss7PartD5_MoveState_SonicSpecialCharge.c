#include <game/Stage/stageobj.h>

typedef struct {
    u8 pad0[0x114];
    s32 unk114;
} UnkStruct_Light;

typedef struct {
    UnkStruct_StageObj base;
    u32 unk20C;
    u8 pad210[0x220 - 0x210];
    u32 unk220;
} UnkStruct_Boss;

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
    u8 pad20C[0x440 - 0x20C];
    void *unk440;
    u8 pad444[0x468 - 0x444];
    s16 unk468;
    u8 pad46A[2];
    s32 unk46C;
} UnkStruct_022e7d14;

extern UnkStruct_Boss *gBoss7;

void Boss7PartD5_UpdateCircleMotion(UnkStruct_022e7d14 *w);
UnkStruct_Fx *BossEffect_CreateHoming(s32 a, s32 x, s32 y, s32 z);
void Boss7PartD5_MoveState_StartAirDash(void);

#define FX_MUL(a, b) ((s32)(((s64)(a) * (b) + 0x800) >> 12))

void Boss7PartD5_MoveState_SonicSpecialCharge(UnkStruct_022e7d14 *w) {
    UnkStruct_Boss *b = gBoss7;
    UnkStruct_Fx *fx;

    w->base.spr0.unk2A += 4;
    w->base.spr1.unk2A += 4;
    ((UnkStruct_Light *)w->base.unk124)->unk114 += 0x400;
    if (w->base.spr0.unk2A >= 0x80) {
        w->base.spr0.unk2A = 0x80;
        w->base.spr1.unk2A = 0x80;
        ((UnkStruct_Light *)w->base.unk124)->unk114 = 0x8000;
    }
    w->unk468 -= 0x20;
    if (w->unk468 < 0)
        w->unk468 = 0;
    w->unk46C -= 0x666;
    if (w->unk46C < 0)
        w->unk46C = 0;
    Boss7PartD5_UpdateCircleMotion(w);
    if (!(b->unk220 & 0xf))
        BossEffect_CreateHoming(1, w->base.unk10, w->base.unk14, w->base.unk18 + 8);
    if (w->base.spr0.unk2A != 0x80)
        return;
    if (w->unk46C != 0)
        return;
    b = gBoss7;
    w->base.spr0.unk2A = 0x80;
    w->base.spr1.unk2A = 0x80;
    ((UnkStruct_Light *)w->base.unk124)->unk114 = 0x8000;
    b->unk20C &= ~0x20;
    b->unk20C |= 0x10;
    fx = BossEffect_CreateHoming(1, w->base.unk10, w->base.unk14, w->base.unk18 + 8);
    fx->unk248 = 0x999;
    fx->unk14C = FX_MUL(fx->unk14C, 0x1800);
    fx->unk150 = FX_MUL(fx->unk150, 0x1800);
    fx->unk154 = FX_MUL(fx->unk154, 0x1800);
    w->unk440 = Boss7PartD5_MoveState_StartAirDash;
}
