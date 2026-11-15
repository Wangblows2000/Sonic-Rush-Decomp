#include <ov000_b/stageobj.h>

extern void *data_ov000_02369ba0;
extern u8 data_ov000_02366cdc[], data_ov000_0236a0ec[], data_ov000_0236a0f4[], data_ov000_0236a104[];

void func_ov000_022fb9c8(void);
void func_ov000_02310e7c(UnkStruct_StageObj *w);

UnkStruct_StageObj *func_ov000_02310e8c(s32 a, s32 b, s32 c) {
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
    func_02069f1c(w, data_ov000_02366cdc, 0, data_ov000_0236a0ec, data_ov000_02369ba0);
    func_0206a054(w, 0, 0x3b);
    if (w->unk1E0->unk2 == 0xb1) {
        func_02069eb0(w, 6, data_ov000_0236a0f4);
    } else {
        func_02069eb0(w, 5, data_ov000_0236a104);
        func_02069818(w, 1);
    }
    func_02069918(w, 0x16);
    func_0206e2b4(&w->coll1, 0x13, -1);
    w->coll1.unk1C = NULL;
    if (w->unk1E0->unk4 & 1)
        w->unk5C ^= 1;
    w->unk1F4 |= 0x20;
    func_ov000_02310e7c(w);
    return w;
}
