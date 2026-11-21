#include <game/Stage/stageobj_alt.h>

typedef struct {
    u8 pad0[0xA];
    u8 unkA;
    u8 unkB;
    u8 padC[0xDC - 0xC];
    u32 unkDC;
    u8 padE0[0x104 - 0xE0];
} UnkStruct_023275c0_Sub;

typedef struct {
    UnkStruct_EffObj base;
    UnkStruct_023275c0_Sub unk134;
} UnkStruct_023275c0;

extern void *gZoneActArchive;
extern u8 data_ov000_02367bcc[], data_ov000_02367be4[];

void StageObj_Load3dBAC(void *w, UnkStruct_023275c0_Sub *s, void *a, s32 b, s32 c, void *d, void *e);
void EffBoostA2_3D_State_DestroyOnAnimEnd(void);

UnkStruct_023275c0 *EffBoostA2_3D_Create(UnkStruct_StageObj *arg, s32 type, s32 x, s32 y, s16 z) {
    UnkStruct_Task *task;
    UnkStruct_023275c0 *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, StageObj_Destructor, 0, 0xa00, 4, sizeof(UnkStruct_023275c0));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_023275c0));
    if (type == 0)
        StageObj_Load3dBAC(w, &w->unk134, data_ov000_02367bcc, 0x1000, 0x10, NULL, gZoneActArchive);
    else
        StageObj_Load3dBAC(w, &w->unk134, data_ov000_02367be4, 0x1800, 0x10, NULL, gZoneActArchive);
    w->unk134.unkA = 7;
    w->unk134.unkB = 0;
    w->unk134.unkDC |= 0x80;
    w->unk134.unkDC |= 0x40;
    w->unk134.unkDC = (w->unk134.unkDC & ~0x1f0000) | 0x180000;
    if (arg->unk5C & 1)
        w->base.unk5C |= 1;
    StageObj_SetAnimation(w, 0);
    w->base.unk88 = arg;
    w->base.unk10 = arg->unk10;
    w->base.unk14 = arg->unk14;
    w->base.unk2E = 0;
    w->base.unk30 = 0;
    if (w->base.unk5C & 1)
        w->base.unk10 -= x << 8;
    else
        w->base.unk10 += x << 8;
    w->base.unk14 += y << 8;
    w->base.unk18 += z << 8;
    w->base.unk58 |= 0x100;
    w->base.unk5C |= 0x100;
    w->base.unk54 |= 2;
    w->base.unk68 = EffBoostA2_3D_State_DestroyOnAnimEnd;
    return w;
}
