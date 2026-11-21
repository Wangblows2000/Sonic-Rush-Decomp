#include <game/Stage/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    UnkStruct_Blk blk[5];
} UnkStruct_0230ca50;

typedef struct {
    u8 b[5];
} UnkStruct_0230ca50_Tbl;

extern void *data_ov000_02369ba0;
extern UnkStruct_0230ca50_Tbl data_ov000_023669b0;
extern u8 data_ov000_02366ad4[], data_ov000_02369f94[], data_ov000_02369f9c[], data_ov000_02366aec[];

void MapObject_Destructor(void);
void Glider_OnHit(void);
void Glider_Action_Carry(UnkStruct_0230ca50 *w);

UnkStruct_0230ca50 *Glider_Create(UnkStruct_ObjParam *p, s32 b, s32 c) {
    UnkStruct_0230ca50_Tbl tbl = data_ov000_023669b0;
    UnkStruct_Task *task;
    UnkStruct_0230ca50 *w;
    vu16 zero;
    u16 i;

    task = Task_Create(StageObj_Main, MapObject_Destructor, 0, 0x800, 2, sizeof(UnkStruct_0230ca50));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_0230ca50));
    MapObject_Init(w, (s32)p, b, c);
    StageObj_Load2dBAC(w, data_ov000_02366ad4, 0xffff, data_ov000_02369f94, data_ov000_02369ba0);
    StageObj_AllocSpritePalette(w, 0, 0x51);
    StageObj_SetOAMOrder(w, 0x14);
    w->base.spr0.unk2C |= 0x80000;
    for (i = 0; i < 5; i++)
        StageObj_LoadModelSimple(w, &w->blk[i], data_ov000_02366aec, 0, i, tbl.b[i], data_ov000_02369f9c, data_ov000_02369ba0);
    w->base.unk5C |= 0x200;
    Obj_SetVec(&w->base.unk128->unk18, 0x34cc, 0x34cc, 0x34cc);
    w->base.spr0.unk46 = 0x14;
    w->base.spr1.unk46 = 0x14;
    ObjRect_SetBox2D(&w->base.coll0, -16, -36, 16, 32);
    ObjRect_SetAtkDefFlags(&w->base.coll0, 0, -16);
    w->base.coll0.unkA |= 0x800;
    w->base.coll0.unk1C = Glider_OnHit;
    w->base.coll0.unk18 = w;
    w->base.unk58 |= 0x2100;
    w->base.unk204 = 0;
    Glider_Action_Carry(w);
    return w;
}
