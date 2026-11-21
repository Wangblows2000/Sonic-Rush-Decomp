#include <ov000_b/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[4];
} UnkStruct_0231bffc;

extern void *data_ov000_02369ba0;
extern u8 data_ov000_02367208[], data_ov000_0236a380[];

void MapObject_Destructor(void);
void WaterJet_OnHit(void);
void WaterJet_Action_Wait(UnkStruct_0231bffc *w);

UnkStruct_0231bffc *WaterJet_Create(s32 a, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_0231bffc *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, MapObject_Destructor, 0, 0x500, 2, sizeof(UnkStruct_0231bffc));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_0231bffc));
    MapObject_Init(w, a, b, c);
    StageObj_Load2dBAC(w, data_ov000_02367208, 0x60, data_ov000_0236a380, data_ov000_02369ba0);
    StageObj_AllocSpritePalette(w, 0, 0x65);
    w->base.spr0.unk46 = 0x14;
    w->base.spr1.unk46 = 0x14;
    ObjRect_SetAtkDefFlags(&w->base.coll0, 0, -0x10);
    w->base.coll0.unk1C = NULL;
    w->base.coll0.unkA |= 0x800;
    w->base.unk58 |= 0x2100;
    switch (w->base.unk1E0->unk2) {
    case 0x85:
        ObjRect_SetBox2D(&w->base.coll1, 0xc, -0x40, 0x24, 0);
        break;
    case 0x86:
        ObjRect_SetBox2D(&w->base.coll1, -0x40, -0xc, 0, -0x24);
        w->base.unk5C |= 1;
        w->base.unk5C |= 0x100;
        w->base.unk1 = 0xc0;
        break;
    case 0x87:
        ObjRect_SetBox2D(&w->base.coll1, -0x40, -0xc, 0, -0x24);
        w->base.unk5C &= ~1;
        w->base.unk5C |= 0x100;
        w->base.unk1 = 0x40;
        break;
    }
    ObjRect_SetAtkDefFlags(&w->base.coll1, 0, -0x10);
    w->base.coll1.unkA |= 0xc0;
    w->base.coll1.unkA |= 0x800;
    w->base.coll1.unk1C = WaterJet_OnHit;
    w->base.coll1.unk18 = w;
    w->base.unk1DC = Sound_AllocHandle();
    WaterJet_Action_Wait(w);
    return w;
}
