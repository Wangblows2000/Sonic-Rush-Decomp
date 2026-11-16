#include <ov000_b/stageobj.h>

void func_ov000_022fb9c8(void);
void func_ov000_0231bbb0(void);

UnkStruct_StageObj *func_ov000_0231bc2c(UnkStruct_ObjParam *p, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_StageObj *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, func_ov000_022fb9c8, 0, 0x500, 2, sizeof(UnkStruct_StageObj));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_StageObj));
    func_ov000_022fba50(w, (s32)p, b, c);
    ObjRect_SetBox2D(&w->coll0, p->unk6, p->unk7, p->unk8 + p->unk6, p->unk9 + p->unk7);
    ObjRect_SetAtkDefFlags(&w->coll0, 0, -0x10);
    w->coll0.unkA |= 0xc0;
    w->coll0.unk1C = func_ov000_0231bbb0;
    w->coll0.unk18 = w;
    w->unk58 |= 0x2120;
    return w;
}
