#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0x3C2];
    s16 unk3C2;
} UnkStruct_0232ee24;

extern void *data_022c448c;
extern u8 data_ov000_023681a4[], data_ov000_0236a9b8[];

void func_ov000_0232ed48(void);

void func_ov000_0232ee24(UnkStruct_0232ee24 *arg) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;

    if (arg->unk3C2 != 0)
        return;
    task = func_020372d0(func_0206c3d0, func_0206bef4, 0, 0xa00, 4, sizeof(UnkStruct_EffObj));
    if (task == func_02006abc(0))
        return;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    func_02069f1c(w, data_ov000_023681a4, 0xffff, data_ov000_0236a9b8, data_022c448c);
    func_0206a054(w, 3, 0x17);
    func_02069818(w, 3);
    func_02069918(w, 9);
    func_020698f0(w, 1);
    w->unk5C |= 1;
    w->unk88 = arg;
    w->unk58 |= 0x100;
    w->unk58 |= 0x2000;
    w->unk54 |= 2;
    w->unk54 |= 0x10;
    w->unk44 = 0x100;
    w->unk46 = 0x100;
    w->unk68 = func_ov000_0232ed48;
}
