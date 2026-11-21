#include <ov000_c/stageobj.h>

extern void *data_ov000_02369ba0;
extern u8 data_ov000_02366b9c[], data_ov000_0236a02c[];

void JetCatapultPart_State_Countdown(void);

void JetCatapult_CreatePart_ov000_0230e354(void *par, s32 x, s32 y) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, StageObj_Destructor, 0, 0xa00, 4, sizeof(UnkStruct_EffObj));
    if (task == OS_GetArenaLo(0))
        return;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    StageObj_Load2dBAC(w, data_ov000_02366b9c, 0x20, data_ov000_0236a02c, data_ov000_02369ba0);
    StageObj_AllocSpritePalette(w, 0, 0x32);
    StageObj_SetAnimation(w, 1);
    StageObj_SetOAMOrder(w, 0x16);
    w->unk5C |= 1;
    w->unk88 = par;
    w->unk10 = x;
    w->unk14 = y;
    w->unkC = 0xb6;
    w->unk58 |= 0x2100;
    w->unk54 |= 2;
    w->unk54 |= 0x10;
    w->unk68 = JetCatapultPart_State_Countdown;
}
