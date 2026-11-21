#include <game/Stage/stageobj_alt.h>

extern void *data_ov000_02369ba0;
extern u8 data_ov000_02366d30[], data_ov000_0236a15c[];

void MapObject_Destructor(void);
void Parachute_OnHit(void);
void Parachute_Action_Idle(UnkStruct_StageObj *w);

UnkStruct_StageObj *Parachute_Create(s32 a, s32 b, s32 c) {
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
    StageObj_Load2dBAC(w, data_ov000_02366d30, 0xffff, data_ov000_0236a15c, data_ov000_02369ba0);
    StageObj_AllocSpritePalette(w, 3, 0x44);
    StageObj_SetAnimation(w, 3);
    StageObj_SetOAMOrder(w, 6);
    StageObj_SetOAMPriority(w, 1);
    ObjRect_SetAtkDefFlags(&w->coll0, 0, -0x10);
    w->coll0.unk1C = Parachute_OnHit;
    w->coll0.unkA |= 0x800;
    w->coll1.unk18 = NULL;
    if (w->unk1E0->unk4 & 1)
        w->unk5C ^= 1;
    w->unk58 |= 0x2100;
    Parachute_Action_Idle(w);
    return w;
}
