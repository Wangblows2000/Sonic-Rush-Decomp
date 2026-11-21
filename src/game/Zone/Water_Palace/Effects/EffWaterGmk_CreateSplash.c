#include <game/Stage/stageobj_alt.h>

extern void *gZoneActArchive;
extern u8 data_ov000_02367ec4[], data_ov000_0236aa28[];

void Effect_SetMoveUntilAnimEndState(UnkStruct_EffObj *w);

UnkStruct_EffObj *EffWaterGmk_CreateSplash(UnkStruct_EffObj *arg, s32 x, s32 y) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, StageObj_Destructor, 0, 0xa00, 4, sizeof(UnkStruct_EffObj));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    StageObj_Load2dBAC(w, data_ov000_02367ec4, 6, data_ov000_0236aa28, gZoneActArchive);
    StageObj_AllocSpritePalette(w, 0, 7);
    if ((arg->unk28 < 0 ? -arg->unk28 : arg->unk28) > 0x400 || arg->unk2A != 0)
        StageObj_SetAnimation(w, 1);
    StageObj_SetOAMOrder(w, 0x12);
    if (arg->unk5C & 1)
        w->unk5C |= 1;
    w->unk5C ^= 1;
    w->unk10 = arg->unk10 + (x << 8);
    w->unk14 = arg->unk14 + (y << 8);
    w->unk58 |= 0x2100;
    w->unk54 |= 2;
    Effect_SetMoveUntilAnimEndState(w);
    return w;
}
