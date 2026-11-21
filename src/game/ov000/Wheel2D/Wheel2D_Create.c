#include <ov000_b/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    UnkStruct_Blk blk[6];
} UnkStruct_0231d5a4;

extern void *data_ov000_02369ba0;
extern u8 data_ov000_02367274[], data_ov000_0236a3b8[], data_ov000_0236726c[], data_ov000_0236a3b0[], data_ov000_0236728c[];

void MapObject_Destructor(void);
void Wheel2D_OnHit(void);
void Wheel2D_Action_Init(UnkStruct_0231d5a4 *w);

UnkStruct_0231d5a4 *Wheel2D_Create(s32 a, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_0231d5a4 *w;
    vu16 zero;
    u16 i;

    task = Task_Create(StageObj_Main, MapObject_Destructor, 0, 0x500, 2, sizeof(UnkStruct_0231d5a4));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_0231d5a4));
    MapObject_Init(w, a, b, c);
    StageObj_Load2dBAC(w, data_ov000_02367274, 0xffff, data_ov000_0236a3b8, data_ov000_02369ba0);
    StageObj_AllocSpritePalette(w, 0, 0x4d);
    StageObj_SetOAMOrder(w, 0x14);
    w->base.spr0.unk2C |= 0x80000;
    for (i = 0; i < 6; i++)
        StageObj_LoadModelSimple(w, &w->blk[i], data_ov000_0236728c, 0, i, data_ov000_0236726c[i], data_ov000_0236a3b0, data_ov000_02369ba0);
    w->base.unk5C |= 0x200;
    Obj_SetVec(&w->base.unk128->unk18, 0x34cc, 0x34cc, 0x34cc);
    ObjRect_SetBox2D(&w->base.coll0, -0x7c, -0x28, -0x60, 0x18);
    ObjRect_SetAtkDefFlags(&w->base.coll0, 0, -0x10);
    w->base.coll0.unkA |= 0xc0;
    w->base.coll0.unk1C = Wheel2D_OnHit;
    w->base.coll0.unk18 = w;
    w->base.unk58 |= 0x2100;
    w->base.unk204 = 0;
    Wheel2D_Action_Init(w);
    return w;
}
