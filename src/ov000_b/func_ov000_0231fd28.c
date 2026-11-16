#include <ov000_b/stageobj.h>

void MapObject_Destructor(void);
void func_ov000_0231fca0(void);

UnkStruct_StageObj *func_ov000_0231fd28(UnkStruct_ObjParam *p, s32 x, s32 y) {
    UnkStruct_Task *task;
    UnkStruct_StageObj *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, MapObject_Destructor, 0, 0x500, 2, sizeof(UnkStruct_StageObj));
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_StageObj));
    w->unk0 = 3;
    w->unk1E8 = x >> 8;
    w->unk1EC = y >> 8;
    w->unk1E0 = p;
    w->unk1E4 = w->unk1E0->unk0;
    w->unk1E0->unk0 = 0xff;
    w->unk58 |= 0x2100;
    w->unk54 |= 0x10;
    w->unk68 = func_ov000_0231fca0;
    return NULL;
}
