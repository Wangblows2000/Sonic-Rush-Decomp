#include <ov000_b/stageobj.h>

void func_ov000_022fb9c8(void);
void func_ov000_0231d1c0(void);

UnkStruct_StageObj *func_ov000_0231d258(UnkStruct_ObjParam *p, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_StageObj *w;
    vu16 zero;

    task = func_020372d0(func_0206c3d0, func_ov000_022fb9c8, 0, 0x500, 2, sizeof(UnkStruct_StageObj));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_StageObj));
    func_ov000_022fba50(w, (s32)p, b, c);
    func_0206e3c0(&w->coll0, p->unk6, p->unk7, p->unk8 + p->unk6, p->unk9 + p->unk7);
    func_0206e2b4(&w->coll0, 0, -0x10);
    w->coll0.unkA |= 0xc0;
    w->coll0.unk1C = func_ov000_0231d1c0;
    w->coll0.unk18 = w;
    w->unk58 |= 0x2120;
    return w;
}
