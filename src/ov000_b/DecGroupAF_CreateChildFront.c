#include <ov000_b/stageobj.h>

extern void *data_ov000_02369ba0;
extern u8 data_ov000_023672f4[], data_ov000_0236a720[], data_ov000_0236a8a0[];

void DecGroupAFChild_State_FollowParent(void);

UnkStruct_EffObj *DecGroupAF_CreateChildFront(UnkStruct_StageObj *arg) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, StageObj_Destructor, 0, 0x4ff, 4, sizeof(UnkStruct_EffObj));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    StageObj_Load2dBAC(w, data_ov000_023672f4, 0, data_ov000_0236a720, data_ov000_02369ba0);
    StageObj_SetSharedSpriteVram(w, 0x10, data_ov000_0236a8a0);
    StageObj_SetOAMOrder(w, 0x15);
    StageObj_AllocSpritePalette(w, 0, 0x5c);
    StageObj_SetAnimation(w, 1);
    w->unk10 = arg->unk10;
    w->unk14 = arg->unk14;
    w->unk88 = arg;
    w->unk5C |= 1;
    w->unk58 |= 0x2100;
    w->unk54 |= 0x12;
    w->unk68 = DecGroupAFChild_State_FollowParent;
    return w;
}
