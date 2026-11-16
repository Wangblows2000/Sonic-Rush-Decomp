#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
} UnkStruct_0230a688;

extern void *data_ov000_02369ba0;

void func_ov000_022fb9c8();
void func_ov000_0230a04c();

UnkStruct_0230a688 *func_ov000_0230a688(UnkStruct_ObjParam *p, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_0230a688 *w;
    vu16 zero;

    task = func_020372d0(func_0206c3d0, func_ov000_022fb9c8, 0, 0x500, 2, sizeof(UnkStruct_0230a688));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_0230a688));
    func_ov000_022fba50(w, (s32)p, b, c);
    func_0206e3c0(&w->base.coll0, p->unk6, p->unk7, p->unk8 + p->unk6, p->unk9 + p->unk7);
    func_0206e2b4(&w->base.coll0, 0, -16);
    w->base.coll0.unkA |= 0xc0;
    w->base.coll0.unk1C = func_ov000_0230a04c;
    w->base.coll0.unk18 = w;
    w->base.unk58 |= 0x2120;
    switch (w->base.unk1E0->unk2) {
    case 0xb7:
        w->base.unk58 |= 0x8000000;
    case 0xb6:
        w->base.unk58 &= ~0x2000;
        func_0206e3c0(&w->base.coll0, 0, -0xc8, 0x28, 0xc8);
        break;
    }
    return w;
}
