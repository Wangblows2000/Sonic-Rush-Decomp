#include <game/Stage/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    UnkStruct_Blk blk[10];
} UnkStruct_02306ae0;

typedef struct {
    u8 b[10];
} UnkStruct_02306ae0_Tbl;

extern void *data_ov000_02369ba0;
extern UnkStruct_02306ae0_Tbl data_ov000_02366894;
extern u8 data_ov000_023668a0[], data_ov000_02369ebc[], data_ov000_02369eb4[], data_ov000_023668bc[];
extern u8 data_ov000_023661c0[];

void MapObject_Destructor(void);
void CraneSlide_OnHit(void);
void CraneSlide_Action_Init(UnkStruct_02306ae0 *w);

UnkStruct_02306ae0 *CraneSlide_Create(UnkStruct_ObjParam *p, s32 b, s32 c) {
    UnkStruct_02306ae0_Tbl tbl = data_ov000_02366894;
    UnkStruct_Task *task;
    UnkStruct_02306ae0 *w;
    vu16 zero;
    u16 i;

    task = Task_Create(StageObj_Main, MapObject_Destructor, 0, 0xf6, 2, sizeof(UnkStruct_02306ae0));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_02306ae0));
    MapObject_Init(w, (s32)p, b, c);
    StageObj_Load2dBAC(w, data_ov000_023668a0, 0xffff, data_ov000_02369ebc, data_ov000_02369ba0);
    StageObj_AllocSpritePalette(w, 1, 0x4c);
    StageObj_SetOAMOrder(w, 0x14);
    StageObj_SetAnimation(w, 1);
    w->base.spr0.unk2C |= 0x80000;
    for (i = 0; i < 10; i++)
        StageObj_LoadModelSimple(w, &w->blk[i], data_ov000_023668bc, 0, i, tbl.b[i], data_ov000_02369eb4, data_ov000_02369ba0);
    w->blk[5].unkA8 |= 0x80;
    w->blk[6].unkA8 |= 0x80;
    w->blk[7].unkA8 |= 0x80;
    w->blk[8].unkA8 |= 0x80;
    w->blk[9].unkA8 |= 0x80;
    w->base.unk5C |= 0x200;
    Obj_SetVec(&w->base.unk128->unk18, 0x34cc, 0x34cc, 0x34cc);
    w->base.spr0.unk46 = 0x14;
    w->base.spr1.unk46 = 0x14;
    ObjRect_SetBox2D(&w->base.coll0, -4, 0x60, 4, 0x68);
    ObjRect_SetAtkDefFlags(&w->base.coll0, 0, -16);
    w->base.coll0.unkA |= 0x800;
    w->base.coll0.unk1C = CraneSlide_OnHit;
    w->base.coll0.unk18 = w;
    w->base.unk1B8 = w;
    w->base.unk1C8 = data_ov000_023661c0;
    w->base.unk1D8 = 3;
    w->base.unk1DA = 10;
    w->base.unk1D4 = -12;
    w->base.unk1D6 = 0;
    w->base.unk58 |= 0x100;
    w->base.unk204 = 0;
    w->base.unk1DC = Sound_AllocHandle();
    CraneSlide_Action_Init(w);
    return w;
}
