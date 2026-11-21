#include <game/Stage/stageobj_alt.h>

extern void *data_ov000_02369ba0;
extern u8 data_ov000_0236733c[], data_ov000_0236a728[];

void DecFishPart_Action_Swim(UnkStruct_EffObj *w);

UnkStruct_EffObj *DecFishPart_Create(UnkStruct_StageObj *arg, s32 x, s32 y) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, StageObj_Destructor, 0, 0xa00, 4, sizeof(UnkStruct_EffObj));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    StageObj_Load2dBAC(w, data_ov000_0236733c, 0xffff, data_ov000_0236a728, data_ov000_02369ba0);
    StageObj_SetOAMPriority(w, 3);
    StageObj_SetOAMOrder(w, 0x16);
    StageObj_AllocSpritePalette(w, 0, 0x54);
    StageObj_SetAnimation(w, 4);
    w->unk10 = arg->unk10;
    w->unk14 = arg->unk14;
    w->unk88 = arg;
    w->unk10 += x << 8;
    w->unk14 += y << 8;
    w->unk2 = x;
    w->unk3 = y;
    w->unk5C |= arg->unk5C & 1;
    DecFishPart_Action_Swim(w);
    return w;
}
