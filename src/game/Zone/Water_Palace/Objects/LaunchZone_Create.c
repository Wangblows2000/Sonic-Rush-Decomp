#include <game/Stage/stageobj_alt.h>

void MapObject_Destructor(void);
void LaunchZone_OnHit(void);

UnkStruct_StageObj *LaunchZone_Create(UnkStruct_ObjParam *p, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_StageObj *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, MapObject_Destructor, 0, 0x500, 2, sizeof(UnkStruct_StageObj));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_StageObj));
    MapObject_Init(w, (s32)p, b, c);
    ObjRect_SetBox2D(&w->coll0, p->unk6, p->unk7, p->unk8 + p->unk6, p->unk9 + p->unk7);
    ObjRect_SetAtkDefFlags(&w->coll0, 0, -0x10);
    w->coll0.unkA |= 0xc0;
    w->coll0.unk1C = LaunchZone_OnHit;
    w->coll0.unk18 = w;
    w->unk58 |= 0x2120;
    return w;
}
