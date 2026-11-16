#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
} UnkStruct_0230b510;

void func_ov000_022fb9c8(void);
void func_ov000_0230b490(void);
void ObjRect_SetBox3D(UnkStruct_Coll *c, s16 a, s16 b, s16 d, s16 e, s16 f, s16 g);

UnkStruct_0230b510 *func_ov000_0230b510(UnkStruct_ObjParam *p, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_0230b510 *w;
    UnkStruct_ObjParam *q;
    vu16 zero;

    task = Task_Create(StageObj_Main, func_ov000_022fb9c8, 0, 0x500, 2, sizeof(UnkStruct_0230b510));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_0230b510));
    func_ov000_022fba50(w, (s32)p, b, c);
    q = w->base.unk1E0;
    if (q->unk8 == 0 && q->unk9 == 0)
        ObjRect_SetBox3D(&w->base.coll0, -32, -32, 32, 32, -0x100, 0x100);
    else
        ObjRect_SetBox3D(&w->base.coll0, q->unk6, q->unk7, q->unk8 + q->unk6, q->unk9 + q->unk7, -0x100, 0x100);
    w->base.unk18 = -0x4000;
    if (w->base.unk1E0->unk4 & 1)
        w->base.unk18 = -w->base.unk18;
    ObjRect_SetAtkDefFlags(&w->base.coll0, 0, -16);
    w->base.coll0.unkA |= 0xc0;
    w->base.coll0.unk1C = func_ov000_0230b490;
    w->base.coll0.unk18 = w;
    w->base.unk58 |= 0x2120;
    return w;
}
