#include <game/Stage/stageobj_alt.h>

void MapObject_Destructor(void);
void MapObj80_OnHit(void);

UnkStruct_StageObj *MapObj80_Create(s32 a, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_StageObj *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, MapObject_Destructor, 0, 0x500, 2, sizeof(UnkStruct_StageObj));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_StageObj));
    MapObject_Init(w, a, b, c);
    if (w->unk1E0->unk4 & 2) {
        if (w->unk1E0->unk4 & 1)
            ObjRect_SetBox2D(&w->coll0, -10, 0, 0x10, 0x20);
        else
            ObjRect_SetBox2D(&w->coll0, 0x20, 0, 0x38, 0x20);
    } else {
        if (w->unk1E0->unk4 & 1)
            ObjRect_SetBox2D(&w->coll0, -0x10, 0, 0, 0x40);
        else
            ObjRect_SetBox2D(&w->coll0, 0, 0, 0x10, 0x40);
    }
    ObjRect_SetAtkDefFlags(&w->coll0, 0, -0x10);
    w->coll0.unkA |= 0xc0;
    w->coll0.unk1C = MapObj80_OnHit;
    w->coll0.unk18 = w;
    w->unk58 |= 0x2120;
    return w;
}
