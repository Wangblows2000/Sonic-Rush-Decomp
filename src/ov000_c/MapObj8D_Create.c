#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
} UnkStruct_023092e4;

extern void *data_ov000_02369ba0;

void MapObject_Destructor();
void func_ov000_023092cc();

UnkStruct_023092e4 *MapObj8D_Create(UnkStruct_ObjParam *p, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_023092e4 *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, MapObject_Destructor, 0, 0x500, 2, sizeof(UnkStruct_023092e4));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_023092e4));
    MapObject_Init(w, (s32)p, b, c);
    w->base.spr0.unk46 = 0x14;
    w->base.spr1.unk46 = 0x14;
    ObjRect_SetAtkDefFlags(&w->base.coll0, 0, -16);
    w->base.coll0.unk1C = NULL;
    w->base.coll0.unkA |= 0x800;
    w->base.unk4A = 0x300;
    w->base.unk58 |= 0x2100;
    w->base.unk5C |= 0x20;
    func_ov000_023092cc(w);
    return w;
}
