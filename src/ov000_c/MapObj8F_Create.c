#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x228 - 0x20C];
} UnkStruct_02308904;

extern void *data_ov000_02369ba0;

void func_ov000_02308720();
void func_ov000_023088e8();
void MapObj8F_Destructor();

UnkStruct_02308904 *MapObj8F_Create(UnkStruct_ObjParam *p, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_02308904 *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, MapObj8F_Destructor, 0, 0x500, 2, sizeof(UnkStruct_02308904));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_02308904));
    MapObject_Init(w, (s32)p, b, c);
    w->base.spr0.unk46 = 0x14;
    w->base.spr1.unk46 = 0x14;
    ObjRect_SetAtkDefFlags(&w->base.coll0, 0, -16);
    w->base.coll0.unk1C = NULL;
    w->base.coll0.unkA |= 0x800;
    w->base.unk4A = 0x300;
    w->base.unk58 |= 0x2100;
    w->base.unk5C |= 0x20;
    if (w->base.unk1E0->unk4 & 1)
        func_ov000_02308720(w);
    else
        func_ov000_023088e8(w);
    return w;
}
