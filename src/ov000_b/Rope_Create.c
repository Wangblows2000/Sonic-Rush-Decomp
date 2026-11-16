#include <ov000_b/stageobj.h>

extern void *data_ov000_02369ba0;
extern u8 data_ov000_02366e5c[], data_ov000_0236a20c[];

void MapObject_Destructor(void);
void Rope_Draw(void);
void Rope_OnHit(void);
void Rope_Action_Init(UnkStruct_StageObj *w);

UnkStruct_StageObj *Rope_Create(UnkStruct_ObjParam *p, s32 b, s32 c) {
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
    StageObj_Load2dBAC(w, data_ov000_02366e5c, 1, data_ov000_0236a20c, data_ov000_02369ba0);
    StageObj_AllocSpritePalette(w, 0, 9);
    w->spr0.unk46 = 0x14;
    w->spr1.unk46 = 0x14;
    w->unk70 = Rope_Draw;
    StageObj_SetAnimation(w, 0);
    ObjRect_SetBox2D(&w->coll0, p->unk6, p->unk7, p->unk8 + p->unk6, p->unk9 + p->unk7);
    ObjRect_SetAtkDefFlags(&w->coll0, 0, -0x10);
    w->coll0.unkA |= 0xc0;
    w->coll0.unk1C = Rope_OnHit;
    w->coll0.unk18 = w;
    w->unk58 |= 0x2100;
    w->unk5C |= 1;
    w->unk204 = 0;
    Rope_Action_Init(w);
    return w;
}
