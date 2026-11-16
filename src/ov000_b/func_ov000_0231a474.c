#include <ov000_b/stageobj.h>

extern void *data_ov000_02369ba0;
extern u8 data_ov000_02367164[], data_ov000_0236a320[];

void func_ov000_022fb9c8(void);
void func_ov000_0231a364(void);
void func_ov000_0231a450(UnkStruct_StageObj *w);

UnkStruct_StageObj *func_ov000_0231a474(s32 a, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_StageObj *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, func_ov000_022fb9c8, 0, 0x500, 2, sizeof(UnkStruct_StageObj));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_StageObj));
    func_ov000_022fba50(w, a, b, c);
    StageObj_Load2dBAC(w, data_ov000_02367164, 0x12, data_ov000_0236a320, data_ov000_02369ba0);
    StageObj_AllocSpritePalette(w, 0, 2);
    w->spr0.unk46 = 0x14;
    w->spr1.unk46 = 0x14;
    ObjRect_SetAtkDefFlags(&w->coll0, 0, -0x10);
    w->coll0.unk1C = func_ov000_0231a364;
    w->coll0.unkA |= 0x800;
    w->unk58 |= 0x2100;
    func_ov000_0231a450(w);
    return w;
}
