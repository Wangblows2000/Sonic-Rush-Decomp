#include <ov000_b/stageobj.h>

extern void *data_ov000_02369ba0;
extern u8 data_ov000_02366d30[], data_ov000_0236a15c[];

void func_ov000_022fb9c8(void);
void func_ov000_02312014(void);
void func_ov000_02312070(UnkStruct_StageObj *w);

UnkStruct_StageObj *func_ov000_02312080(s32 a, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_StageObj *w;
    vu16 zero;

    task = func_020372d0(func_0206c3d0, func_ov000_022fb9c8, 0, 0x500, 2, sizeof(UnkStruct_StageObj));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_StageObj));
    func_ov000_022fba50(w, a, b, c);
    func_02069f1c(w, data_ov000_02366d30, 0xffff, data_ov000_0236a15c, data_ov000_02369ba0);
    func_0206a054(w, 3, 0x44);
    func_02069818(w, 3);
    func_02069918(w, 6);
    func_020698f0(w, 1);
    func_0206e2b4(&w->coll0, 0, -0x10);
    w->coll0.unk1C = func_ov000_02312014;
    w->coll0.unkA |= 0x800;
    w->coll1.unk18 = NULL;
    if (w->unk1E0->unk4 & 1)
        w->unk5C ^= 1;
    w->unk58 |= 0x2100;
    func_ov000_02312070(w);
    return w;
}
