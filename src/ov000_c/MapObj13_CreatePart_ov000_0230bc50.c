#include <ov000_c/stageobj.h>

extern void *gActComArchive;
extern u8 data_ov000_023669b8[], data_ov000_02369f84[];

void MapObj13Part_State_Unk_ov000_0230bc38(void);

UnkStruct_EffObj *MapObj13_CreatePart_ov000_0230bc50(UnkStruct_StageObj *par) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, StageObj_Destructor, 0, 0xa00, 4, sizeof(UnkStruct_EffObj));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    StageObj_Load2dBAC(w, data_ov000_023669b8, 0xffff, data_ov000_02369f84, gActComArchive);
    StageObj_AllocSpritePalette(w, 0, 0x67);
    StageObj_SetAnimation(w, 1);
    StageObj_SetOAMOrder(w, 9);
    StageObj_SetOAMPriority(w, 1);
    w->unk5C |= 1;
    w->unk10 = par->unk10;
    w->unk14 = par->unk14;
    w->unk58 |= 0x100;
    w->unk54 |= 2;
    w->unk68 = MapObj13Part_State_Unk_ov000_0230bc38;
    return w;
}
