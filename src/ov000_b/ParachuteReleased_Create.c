#include <ov000_b/stageobj.h>

extern void *data_ov000_02369ba0;
extern u8 data_ov000_02366d14[], data_ov000_0236a15c[];

void func_ov000_02311cc4(void);

void ParachuteReleased_Create(UnkStruct_StageObj *arg) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, StageObj_Destructor, 0, 0xa00, 4, sizeof(UnkStruct_EffObj));
    if (task == OS_GetArenaLo(0))
        return;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    StageObj_Load2dBAC(w, data_ov000_02366d14, 0xffff, data_ov000_0236a15c, data_ov000_02369ba0);
    StageObj_AllocSpritePalette(w, 0, 0x3d);
    StageObj_SetOAMOrder(w, 0xa);
    w->spr0.unk2C |= 0x200;
    w->spr1.unk2C |= 0x200;
    w->unk88 = arg;
    w->unk10 = arg->unk10;
    w->unk14 = arg->unk14;
    w->unkC = 0;
    if (((UnkStruct_StageObj *)w->unk88)->unk5C & 1)
        w->unk5C |= 1;
    else
        w->unk5C &= ~1;
    w->unk5C |= 4;
    w->unk5C |= 0x100;
    w->unk58 |= 0x2100;
    w->unk54 |= 2;
    w->unk54 |= 0x10;
    w->unk68 = func_ov000_02311cc4;
}
