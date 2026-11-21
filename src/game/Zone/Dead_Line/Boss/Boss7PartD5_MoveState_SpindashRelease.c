#include <game/Stage/stageobj.h>

typedef struct {
    u8 pad0[0x14C];
    s32 unk14C;
    s32 unk150;
    s32 unk154;
    u8 pad158[0x248 - 0x158];
    s32 unk248;
} UnkStruct_Fx;

typedef struct {
    u32 unk0;
    s32 unk4;
} UnkStruct_02369ac0;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x3D0 - 0x20C];
    s32 unk3D0;
    u8 pad3D4[0x440 - 0x3D4];
    void *unk440;
    u8 pad444[0x480 - 0x444];
    void *unk480;
    u8 pad484[4];
    void *unk488;
} UnkStruct_022e8e6c;

extern UnkStruct_02369ac0 gBossStatus;
extern s16 data_ov000_02368b86[][24];

void Boss7_SetRivalAnimation(UnkStruct_022e8e6c *w, s32 a);
void Boss7_SetCollidersEnabled(UnkStruct_022e8e6c *w, s32 a, s32 b);
void *EffTrail_Create(UnkStruct_022e8e6c *w, s32 a, s32 b, s32 c, s32 d);
UnkStruct_Fx *BossEffect_CreateHoming(s32 a, s32 x, s32 y, s32 z);
void *EffBoostA_3D_Create(UnkStruct_022e8e6c *w, s32 a, s32 b, s32 c, s32 d);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void Boss7PartD5_MoveState_Roll(UnkStruct_022e8e6c *w);

static inline s32 Mul64(s64 a, s64 b) {
    return (s32)((a * b + 0x800) >> 12);
}

void Boss7PartD5_MoveState_SpindashRelease(UnkStruct_022e8e6c *w) {
    UnkStruct_Fx *fx;

    w->base.unk58 |= 0x10;
    if (w->base.unk5C & 1)
        w->base.unk2E = -data_ov000_02368b86[w->unk3D0][0];
    else
        w->base.unk2E = data_ov000_02368b86[w->unk3D0][0];
    Boss7_SetRivalAnimation(w, 0x31);
    Boss7_SetCollidersEnabled(w, 0, 1);
    w->base.unk5C |= 4;
    if (w->unk3D0 == 0) {
        w->unk480 = EffTrail_Create(w, 0x18, 0xa, 0x7ca5, 0x7ca5);
        fx = BossEffect_CreateHoming(1, w->base.unk10, w->base.unk14, w->base.unk18 + 8);
        fx->unk248 = 0x999;
        fx->unk14C = Mul64(fx->unk14C, 0x1800);
        fx->unk150 = Mul64(fx->unk150, 0x1800);
        fx->unk154 = Mul64(fx->unk154, 0x1800);
        w->unk488 = EffBoostA_3D_Create(w, 0, -4, 8, 0);
        Sound_PlaySeqArc(0, 0, 1);
    } else {
        Sound_PlaySeqArc(0, 0, 0x12);
    }
    gBossStatus.unk4 = 0x2a;
    w->unk440 = Boss7PartD5_MoveState_Roll;
    ((void (*)(UnkStruct_022e8e6c *))w->unk440)(w);
}
