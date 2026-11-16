#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0xA];
    u8 unkA;
    u8 unkB;
    u8 padC[0x104 - 0xC];
} UnkStruct_02331670_Sub;

typedef struct {
    UnkStruct_EffObj base;
    UnkStruct_02331670_Sub unk134;
} UnkStruct_02331670;

extern void *gZoneActArchive;
extern u8 data_ov000_0236844c[];
extern u8 data_ov000_0236a9f8[];

s32 FX_Atan2(s32 y, s32 x);
void Math_RotateVec2S16(s16 *x, s16 *y, u8 a);
void func_ov000_0232e90c(UnkStruct_02331670 *w);
void StageObj_Load3dBAC(void *w, UnkStruct_02331670_Sub *s, void *a, s32 b, s32 c, void *d, void *e);

UnkStruct_02331670 *EffBoostJet3D_Create(UnkStruct_StageObj *arg, s16 x, s16 y, s32 unused) {
    UnkStruct_Task *task;
    UnkStruct_02331670 *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, StageObj_Destructor, 0, 0xa00, 4, sizeof(UnkStruct_02331670));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_02331670));
    StageObj_Load3dBAC(w, &w->unk134, data_ov000_0236844c, 8, 0x10, data_ov000_0236a9f8, gZoneActArchive);
    w->unk134.unkA = 0x14;
    w->unk134.unkB = 7;
    w->base.unk88 = arg;
    if (arg->unk5C & 1)
        x = -x;
    w->base.unk10 = arg->unk10;
    w->base.unk14 = arg->unk14;
    w->base.unk1 = FX_Atan2(-0x800, ((UnkStruct_EffObj *)w->base.unk88)->unk2E) >> 8;
    w->base.unk1 += arg->unk5 + 0x40;
    Math_RotateVec2S16(&x, &y, arg->unk1 + arg->unk5);
    w->base.unk2E = x;
    w->base.unk30 = y;
    w->base.unk10 += x << 8;
    w->base.unk14 += y << 8;
    w->base.unk58 |= 0x2100;
    w->base.unk5C |= 0x100;
    w->base.unk54 |= 2;
    func_ov000_0232e90c(w);
    return w;
}
