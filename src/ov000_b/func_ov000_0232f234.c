#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0x3C0];
    s16 unk3C0;
} UnkStruct_0232f234;

extern void *data_022c448c;
extern u8 data_ov000_023681cc[], data_ov000_0236a9b8[];

void func_ov000_0232f1cc(void);

void func_ov000_0232f234(UnkStruct_0232f234 *arg) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;

    if (arg->unk3C0 != 0)
        return;
    task = func_020372d0(func_0206c3d0, func_0206bef4, 0, 0xa00, 4, sizeof(UnkStruct_EffObj));
    if (task == func_02006abc(0))
        return;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    func_02069f1c(w, data_ov000_023681cc, 0xffff, data_ov000_0236a9b8, data_022c448c);
    func_0206a054(w, 1, 2);
    func_02069818(w, 1);
    func_02069918(w, 9);
    func_020698f0(w, 1);
    w->unk5C |= 1;
    w->unk5C |= 4;
    w->unk88 = arg;
    w->unk58 |= 0x100;
    w->unk58 |= 0x2000;
    w->unk54 |= 2;
    w->unk54 |= 0x10;
    w->unk68 = func_ov000_0232f1cc;
}
