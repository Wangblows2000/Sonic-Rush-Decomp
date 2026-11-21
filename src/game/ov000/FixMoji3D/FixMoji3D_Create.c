#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0xA];
    u8 unkA;
    u8 unkB;
    u8 padC[0x104 - 0xC];
} UnkStruct_0232a9d0_Sub;

typedef struct {
    UnkStruct_EffObj base;
    UnkStruct_0232a9d0_Sub unk134;
} UnkStruct_0232a9d0;

extern void *gZoneActArchive;
extern u8 data_ov000_02367f90[];

void FixMoji3D_State_FollowParent(void);
void StageObj_Load3dBAC(void *w, UnkStruct_0232a9d0_Sub *s, void *a, s32 b, s32 c, void *d, void *e);
void Model_Animate(UnkStruct_0232a9d0_Sub *s);
void BossArena_ArcToCirclePos(s32 a, s32 b, s32 c, s32 *d, s32 *e, s16 *f);

UnkStruct_0232a9d0 *FixMoji3D_Create(UnkStruct_StageObj *arg, s32 x, s32 y, u32 idx, s32 a, s32 b) {
    UnkStruct_Task *task;
    UnkStruct_0232a9d0 *w;
    s32 o10;
    s32 o14;
    s16 o0C;
    vu16 zero;

    if (idx >= 10)
        return NULL;
    task = Task_Create(StageObj_Main, StageObj_Destructor, 0, 0xa00, 4, sizeof(UnkStruct_0232a9d0));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_0232a9d0));
    StageObj_Load3dBAC(w, &w->unk134, data_ov000_02367f90, 0x80, 0x10, NULL, gZoneActArchive);
    StageObj_SetAnimation(w, (u16)idx);
    w->unk134.unkA = 0x14;
    w->unk134.unkB = 7;
    w->base.unk54 |= 0x10;
    w->base.unk5C |= 0x1000;
    w->base.unk5C |= 1;
    Model_Animate(&w->unk134);
    w->base.unk2E = x << 8;
    w->base.unk30 = y << 8;
    w->base.unk10 = arg->unk10 + w->base.unk2E;
    w->base.unk14 = arg->unk14 + w->base.unk30;
    w->base.unk88 = arg;
    w->base.unk68 = FixMoji3D_State_FollowParent;
    w->base.unkC = 0;
    w->base.unk8 = 0;
    w->base.unk58 |= 0x2100;
    w->base.unk54 |= 2;
    BossArena_ArcToCirclePos(a, b, w->base.unk10 << 4, &o10, &o14, &o0C);
    w->base.unk10 = o10 >> 4;
    w->base.unk18 = o14 >> 4;
    return w;
}
