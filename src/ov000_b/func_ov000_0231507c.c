#include <ov000_b/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    UnkStruct_Blk blk;
} UnkStruct_0231507c;

extern void *data_ov000_02369ba0;
extern u8 data_ov000_02366eb0[], data_ov000_0236a22c[], data_ov000_0236a214[], data_ov000_02366ec8[];

void func_ov000_022fb9c8(void);
void func_ov000_02314dc8(void);
void func_ov000_02314d0c(UnkStruct_0231507c *w);
void func_ov000_0231506c(UnkStruct_0231507c *w);

UnkStruct_0231507c *func_ov000_0231507c(s32 a, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_0231507c *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, func_ov000_022fb9c8, 0, 0x500, 2, sizeof(UnkStruct_0231507c));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_0231507c));
    func_ov000_022fba50(w, a, b, c);
    StageObj_Load2dBAC(w, data_ov000_02366eb0, 0xffff, data_ov000_0236a22c, data_ov000_02369ba0);
    StageObj_AllocSpritePalette(w, 0, 0x4b);
    StageObj_SetOAMOrder(w, 0x14);
    w->base.spr0.unk2C |= 0x80000;
    StageObj_LoadModelSimple(w, &w->blk, data_ov000_02366ec8, 1, 0, 0, data_ov000_0236a214, data_ov000_02369ba0);
    w->base.unk5C |= 0x200;
    Obj_SetVec(&w->base.unk128->unk18, 0x34cc, 0x34cc, 0x34cc);
    if (w->base.unk1E0->unk4 & 1)
        ObjRect_SetBox2D(&w->base.coll0, 0x84, 0x40, 0x8c, 0x48);
    else
        ObjRect_SetBox2D(&w->base.coll0, -0x88, 0x40, -0x80, 0x48);
    ObjRect_SetAtkDefFlags(&w->base.coll0, 0, -0x10);
    w->base.coll0.unkA |= 0xc0;
    w->base.coll0.unk1C = func_ov000_02314dc8;
    w->base.coll0.unk18 = w;
    w->base.unk58 |= 0x2100;
    w->base.unk204 = 0;
    func_ov000_02314d0c(w);
    func_ov000_0231506c(w);
    return w;
}
