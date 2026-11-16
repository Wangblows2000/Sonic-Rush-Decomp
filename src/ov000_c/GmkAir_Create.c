#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
} UnkStruct_02301ba4;

extern void *data_ov000_02369ba0;
extern u8 data_ov000_023666d0[], data_ov000_02369e44[];

void MapObject_Destructor(void);
void GmkAir_OnHit(void);
BOOL Stage_IsGravityFlipped(s32 x, s32 y);

UnkStruct_02301ba4 *GmkAir_Create(UnkStruct_ObjParam *p, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_02301ba4 *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, MapObject_Destructor, 0, 0x500, 2, sizeof(UnkStruct_02301ba4));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_02301ba4));
    MapObject_Init(w, (s32)p, b, c);
    StageObj_Load2dBAC(w, data_ov000_023666d0, 0xffff, data_ov000_02369e44, data_ov000_02369ba0);
    StageObj_AllocSpritePalette(w, 0, 0x2a);
    w->base.spr0.unk46 = 0x14;
    w->base.spr1.unk46 = 0x14;
    ObjRect_SetBox2D(&w->base.coll0, p->unk6, p->unk7 - 0x60, p->unk8 + p->unk6, p->unk9 + p->unk7);
    ObjRect_SetAtkDefFlags(&w->base.coll0, 0, -16);
    w->base.coll0.unkA |= 0xc0;
    w->base.coll0.unk1C = GmkAir_OnHit;
    w->base.coll0.unk18 = w;
    w->base.unk58 |= 0x2100;
    if (Stage_IsGravityFlipped(w->base.unk10, w->base.unk14))
        w->base.unk5C |= 2;
    return w;
}
