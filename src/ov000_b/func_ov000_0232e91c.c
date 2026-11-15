#include <ov000_b/stageobj.h>

extern void *data_022c448c;
extern u8 data_ov000_02368170[], data_ov000_0236a9e8[];

void func_020693f0(s16 *x, s16 *y, u8 a);
void func_ov000_0232e90c(UnkStruct_EffObj *w);

void func_ov000_0232e91c(UnkStruct_StageObj *arg, s16 x, s16 y, s32 type) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;

    task = func_020372d0(func_0206c3d0, func_0206bef4, 0, 0xa00, 4, sizeof(UnkStruct_EffObj));
    if (task == func_02006abc(0))
        return;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    func_02069f1c(w, data_ov000_02368170, 8, data_ov000_0236a9e8, data_022c448c);
    func_0206a054(w, 0, 2);
    func_02069818(w, (u16)(type - 1));
    func_02069918(w, 9);
    if (arg->unk5 != 0) {
        w->spr0.unk2C |= 0x200;
        w->spr1.unk2C |= 0x200;
    }
    w->unk5C |= 1;
    w->unk88 = arg;
    w->unk10 = arg->unk10;
    w->unk14 = arg->unk14;
    w->unk1 = arg->unk5;
    func_020693f0(&x, &y, arg->unk5);
    w->unk2E = x;
    w->unk30 = y;
    w->unkC = 0x3c;
    w->unk62 = 0x80;
    w->unk58 |= 0x2100;
    w->unk54 |= 2;
    w->unk5C |= 0x100;
    func_ov000_0232e90c(w);
}
