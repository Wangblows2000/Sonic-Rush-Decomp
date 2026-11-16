#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
} UnkStruct_0230429c;

extern void *data_ov000_02369ba0;

void MapObj74_Action_Unk_ov000_02304270();
void MapObj74_Destructor();
void func_ov000_02342780();

UnkStruct_0230429c *MapObj74_Create(UnkStruct_ObjParam *p, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_0230429c *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, MapObj74_Destructor, 0, 0x500, 2, sizeof(UnkStruct_0230429c));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_0230429c));
    MapObject_Init(w, (s32)p, b, c);
    w->base.unk8 = 0;
    w->base.unk54 |= 0x10;
    w->base.unk58 |= 0x2100;
    w->base.unk5C |= 0x20;
    MapObj74_Action_Unk_ov000_02304270(w);
    func_ov000_02342780();
    return w;
}
