#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0xA];
    u8 unkA;
    u8 unkB;
    u8 padC[0x104 - 0xC];
} UnkStruct_02329128_Sub;

typedef struct {
    UnkStruct_EffObj base;
    UnkStruct_02329128_Sub unk134;
} UnkStruct_02329128;

extern void *gZoneActArchive;
extern u8 data_ov000_02367ee0[], data_ov000_0236aa20[];

void EffWater3D_State_DestroyOnAnimEnd(void);
void StageObj_Load3dBAC(void *w, UnkStruct_02329128_Sub *s, void *a, s32 b, s32 c, void *d, void *e);
void BossArena_ArcToCirclePos(s32 a, s32 b, s32 c, s32 *d, s32 *e, s16 *f);

UnkStruct_02329128 *EffWater3D_Create(UnkStruct_StageObj *arg, s32 x, s32 y, s32 unused, s32 a, s32 b) {
    UnkStruct_Task *task;
    UnkStruct_02329128 *w;
    s32 o10;
    s32 o14;
    s16 o0C;
    vu16 zero;

    task = Task_Create(StageObj_Main, StageObj_Destructor, 0, 0xa00, 4, sizeof(UnkStruct_02329128));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_02329128));
    StageObj_Load3dBAC(w, &w->unk134, data_ov000_02367ee0, 0x400, 0x10, data_ov000_0236aa20, gZoneActArchive);
    StageObj_SetAnimation(w, 2);
    w->unk134.unkA = 0x14;
    w->unk134.unkB = 7;
    if (arg->unk5C & 1)
        w->base.unk5C |= 1;
    w->base.unk10 = arg->unk10 + (x << 8);
    w->base.unk14 = arg->unk14 + (y << 8);
    w->base.unk58 |= 0x100;
    w->base.unk54 |= 2;
    w->base.unk68 = EffWater3D_State_DestroyOnAnimEnd;
    BossArena_ArcToCirclePos(a, b, w->base.unk10 << 4, &o10, &o14, &o0C);
    w->base.unk10 = o10 >> 4;
    w->base.unk18 = o14 >> 4;
    return w;
}
