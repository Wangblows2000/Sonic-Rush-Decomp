#include <ov000_b/stageobj.h>

extern void *data_ov000_02369ba0;
extern u8 data_ov000_02367164[], data_ov000_0236a320[];

void MapObject_Destructor(void);
void Trampoline_OnHit(void);
void Trampoline_Action_Idle(UnkStruct_StageObj *w);

UnkStruct_StageObj *Trampoline_Create(s32 a, s32 b, s32 c) {
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
    StageObj_Load2dBAC(w, data_ov000_02367164, 0x12, data_ov000_0236a320, data_ov000_02369ba0);
    StageObj_AllocSpritePalette(w, 0, 2);
    w->spr0.unk46 = 0x14;
    w->spr1.unk46 = 0x14;
    ObjRect_SetAtkDefFlags(&w->coll0, 0, -0x10);
    w->coll0.unk1C = Trampoline_OnHit;
    w->coll0.unkA |= 0x800;
    w->unk58 |= 0x2100;
    Trampoline_Action_Idle(w);
    return w;
}
