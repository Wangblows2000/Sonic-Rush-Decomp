#include <game/Stage/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
} UnkStruct_0230a688;

extern void *data_ov000_02369ba0;

void MapObject_Destructor();
void MapObj40_OnHit();

UnkStruct_0230a688 *MapObj40_Create(UnkStruct_ObjParam *p, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_0230a688 *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, MapObject_Destructor, 0, 0x500, 2, sizeof(UnkStruct_0230a688));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_0230a688));
    MapObject_Init(w, (s32)p, b, c);
    ObjRect_SetBox2D(&w->base.coll0, p->unk6, p->unk7, p->unk8 + p->unk6, p->unk9 + p->unk7);
    ObjRect_SetAtkDefFlags(&w->base.coll0, 0, -16);
    w->base.coll0.unkA |= 0xc0;
    w->base.coll0.unk1C = MapObj40_OnHit;
    w->base.coll0.unk18 = w;
    w->base.unk58 |= 0x2120;
    switch (w->base.unk1E0->unk2) {
    case 0xb7:
        w->base.unk58 |= 0x8000000;
    case 0xb6:
        w->base.unk58 &= ~0x2000;
        ObjRect_SetBox2D(&w->base.coll0, 0, -0xc8, 0x28, 0xc8);
        break;
    }
    return w;
}
