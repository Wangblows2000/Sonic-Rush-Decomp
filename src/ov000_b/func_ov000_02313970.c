#include <ov000_b/stageobj.h>

extern void *data_ov000_02369ba0;
extern u8 data_ov000_02366df8[], data_ov000_0236a1bc[], data_ov000_023661c0[];

void func_ov000_022fb9c8(void);
void func_ov000_02313854(void);
void func_ov000_0231394c(UnkStruct_StageObj *w);

UnkStruct_StageObj *func_ov000_02313970(s32 a, s32 b, s32 c) {
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
    func_02069f1c(w, data_ov000_02366df8, 0x10, data_ov000_0236a1bc, data_ov000_02369ba0);
    func_0206a054(w, 0, 6);
    w->spr0.unk46 = 0x12;
    w->spr1.unk46 = 0x12;
    func_0206e2b4(&w->coll0, 0, 8);
    w->coll0.unk1C = func_ov000_02313854;
    w->unk58 |= 0x2100;
    w->unk1B8 = w;
    w->unk1C8 = data_ov000_023661c0;
    w->unk1D8 = 5;
    w->unk1DA = 4;
    w->unk1D4 = -0x12;
    w->unk1D6 = -0x10;
    func_ov000_0231394c(w);
    return w;
}
