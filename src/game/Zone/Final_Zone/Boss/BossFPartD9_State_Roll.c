#include <game/Stage/stageobj.h>

typedef struct {
    u8 pad[0x20C];
    u32 unk20C;
} UnkStruct_Mgr;

typedef struct {
    UnkStruct_StageObj base;
    u16 unk20C;
} UnkStruct_022f7ccc;

extern UnkStruct_Mgr *gBossF;

void BossEffect_CreateBombBall(s32 a, s32 x, s32 y, s32 z);

void BossFPartD9_State_Roll(UnkStruct_022f7ccc *w) {
    UnkStruct_Mgr *m = gBossF;

    w->unk20C -= w->base.unk2E * 2;
    if (w->base.unk58 & 1)
        w->base.unk58 &= ~0x10;
    else
        w->base.unk58 |= 0x10;
    if ((!(w->base.unk58 & 1) || w->base.unk80 != NULL) && (m->unk20C & 0x40) && !(w->base.unk54 & 8))
        return;
    w->base.unk54 |= 4;
    BossEffect_CreateBombBall(1, w->base.unk10, w->base.unk14, w->base.unk18);
}
