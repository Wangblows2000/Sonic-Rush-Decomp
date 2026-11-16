#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
} UnkStruct_0230b344;

extern void *data_ov000_02369ba0;
typedef struct {
    u8 pad0[0x1C];
    u8 unk1C;
    u8 unk1D;
} UnkStruct_022c4560;
extern UnkStruct_022c4560 data_022c4560;
extern u8 data_ov000_02366968[];
extern u8 data_ov000_02369f54[];

void MapObject_Destructor();
void GoalRing_OnHit();
void func_ov000_0230b320();

UnkStruct_0230b344 *GoalRing_Create(UnkStruct_ObjParam *p, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_0230b344 *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, MapObject_Destructor, 0, 0x500, 2, sizeof(UnkStruct_0230b344));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_0230b344));
    MapObject_Init(w, (s32)p, b, c);
    StageObj_Load2dBAC(w, data_ov000_02366968, 0x20, data_ov000_02369f54, data_ov000_02369ba0);
    StageObj_AllocSpritePalette(w, 0, 4);
    StageObj_SetOAMOrder(w, 0x14);
    if (data_022c4560.unk1C == 2 && data_022c4560.unk1D != 0)
        StageObj_SetOAMPriority(w, 1);
    ObjRect_SetAtkDefFlags(&w->base.coll0, 0, -16);
    w->base.coll0.unk1C = GoalRing_OnHit;
    w->base.unk58 |= 0x2100;
    w->base.unk1DC = Sound_AllocHandle();
    func_ov000_0230b320(w);
    return w;
}
