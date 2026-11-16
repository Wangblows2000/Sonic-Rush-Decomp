#include <ov000_b/stageobj.h>

extern void *gActComArchive;
extern u8 data_ov000_02368270[], data_ov000_0236a9b8[];

void func_ov000_02334264(UnkStruct_EffObj *w);

UnkStruct_EffObj *EffVs_Create(s32 x, s32 y) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, StageObj_Destructor, 0, 0xa00, 4, sizeof(UnkStruct_EffObj));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    StageObj_Load2dBAC(w, data_ov000_02368270, 0xffff, data_ov000_0236a9b8, gActComArchive);
    StageObj_AllocSpritePalette(w, 4, 0x17);
    StageObj_SetAnimation(w, 4);
    StageObj_SetOAMOrder(w, 9);
    StageObj_SetOAMPriority(w, 1);
    w->unk5C |= 1;
    w->unk10 = x;
    w->unk14 = y;
    w->unk58 |= 0x100;
    w->unk58 |= 0x2000;
    w->unk54 |= 2;
    func_ov000_02334264(w);
    return w;
}
