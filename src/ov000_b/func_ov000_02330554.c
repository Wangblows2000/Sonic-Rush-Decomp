#include <ov000_b/stageobj.h>

extern void *data_022c448c;
extern u8 data_ov000_023682b4[], data_ov000_0236aa18[];

void func_ov000_023304d4(void);

void func_ov000_02330554(s32 x, s32 y, s32 t) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;

    task = func_020372d0(func_0206c3d0, func_0206bef4, 0, 0xa00, 4, sizeof(UnkStruct_EffObj));
    if (task == func_02006abc(0))
        return;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    func_02069f1c(w, data_ov000_023682b4, 5, data_ov000_0236aa18, data_022c448c);
    func_0206a054(w, 0xc, 0xc);
    func_02069818(w, 0xc);
    func_02069918(w, 9);
    func_020698f0(w, 1);
    w->unk10 = x << 8;
    w->unk14 = y << 8;
    w->unkC = t;
    w->unk58 |= 0x100;
    w->unk54 |= 2;
    w->unk68 = func_ov000_023304d4;
}
