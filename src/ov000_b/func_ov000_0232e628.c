#include <ov000_b/stageobj.h>

extern void *data_022c448c;
extern u32 data_022c4ef0;
extern u8 data_ov000_02368154[], data_ov000_0236aa10[];

void func_020693f0(s16 *x, s16 *y, u8 a);
void func_ov000_02334264(UnkStruct_EffObj *w);

UnkStruct_EffObj *func_ov000_0232e628(UnkStruct_StageObj *arg, s16 x, s16 y, s32 unused) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;

    task = func_020372d0(func_0206c3d0, func_0206bef4, 0, 0xa00, 4, sizeof(UnkStruct_EffObj));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    func_02069f1c(w, data_ov000_02368154, 0x20, data_ov000_0236aa10, data_022c448c);
    func_0206a054(w, 0, 0xc);
    func_02069918(w, 9);
    if (arg->unk5 != 0) {
        w->spr0.unk2C |= 0x200;
        w->spr1.unk2C |= 0x200;
    }
    if (arg->unk5C & 1)
        w->unk5C |= 1;
    w->unk10 = arg->unk10;
    w->unk14 = arg->unk14;
    w->unk18 = arg->unk18;
    if (data_022c4ef0 & 0x100)
        func_020698f0(w, 1);
    w->unk1 = arg->unk5;
    func_020693f0(&x, &y, arg->unk5);
    w->unk10 += x << 8;
    w->unk14 += y << 8;
    w->unk2E = 0;
    w->unk30 = 0;
    w->unk58 |= 0x2100;
    w->unk54 |= 2;
    w->unk5C |= 0x100;
    func_ov000_02334264(w);
    return w;
}
