#include <ov000_b/stageobj.h>

void func_ov000_022fb9c8(void);
void func_ov000_023355e0(u16 a, u16 b, u16 c, u16 d);

UnkStruct_StageObj *func_ov000_02311000(s32 a, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_StageObj *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, func_ov000_022fb9c8, 0, 0xf6, 2, sizeof(UnkStruct_StageObj));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_StageObj));
    func_ov000_022fba50(w, a, b, c);
    w->unk58 |= 0x2100;
    w->unk58 |= 0x100;
    w->unk54 |= 0x20;
    func_ov000_023355e0(w->unk1E8 - 0x80 + (w->unk1E0->unk6 << 8), w->unk1EC - 0x120 + w->unk1E0->unk7 * 0xc0,
                        w->unk1E8 + 0x80 + (w->unk1E0->unk6 << 8), w->unk1EC + 0x120 + w->unk1E0->unk7 * 0xc0);
    return w;
}
