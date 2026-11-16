#include <ov000_b/stageobj.h>

extern void *data_ov000_02369ba0;
extern u8 data_ov000_02366dc4[], data_ov000_0236a1b4[];

void func_ov000_022fb9c8(void);
void func_ov000_02313574(UnkStruct_StageObj *w);

UnkStruct_StageObj *func_ov000_023135b0(s32 a, s32 b, s32 c) {
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
    StageObj_Load2dBAC(w, data_ov000_02366dc4, 0xffff, data_ov000_0236a1b4, data_ov000_02369ba0);
    StageObj_AllocSpritePalette(w, 0, 0x2d);
    w->spr0.unk46 = 0x14;
    w->spr1.unk46 = 0x14;
    w->unk58 |= 0x2100;
    w->unk1DC = Sound_AllocHandle();
    func_ov000_02313574(w);
    return w;
}
