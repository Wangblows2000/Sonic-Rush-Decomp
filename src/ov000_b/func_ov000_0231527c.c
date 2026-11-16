#include <ov000_b/stageobj.h>

extern void *data_ov000_02369ba0;
extern u8 data_ov000_02366ee0[], data_ov000_0236a234[];

void func_ov000_02315278(void);

void func_ov000_0231527c(UnkStruct_StageObj *arg) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, StageObj_Destructor, 0, 0xa00, 4, sizeof(UnkStruct_EffObj));
    if (task == func_02006abc(0))
        return;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    StageObj_Load2dBAC(w, data_ov000_02366ee0, 0xffff, data_ov000_0236a234, data_ov000_02369ba0);
    StageObj_AllocSpritePalette(w, 0, 0x20);
    StageObj_SetAnimation(w, 1);
    w->spr0.unk46 = 0x15;
    w->spr1.unk46 = 0x15;
    w->unk5C |= 1;
    w->unk88 = arg;
    w->unk10 = ((UnkStruct_StageObj *)w->unk88)->unk10;
    w->unk14 = ((UnkStruct_StageObj *)w->unk88)->unk14 + 0x2c00;
    w->unk58 |= 0x2100;
    w->unk54 |= 0x12;
    w->unk68 = func_ov000_02315278;
}
