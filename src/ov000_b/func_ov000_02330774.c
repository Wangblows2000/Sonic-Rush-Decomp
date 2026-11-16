#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
    u8 pad18[0x325 - 0x18];
    u8 unk325;
} UnkStruct_02330774;

void func_ov000_02330670(void);

void func_ov000_02330774(UnkStruct_02330774 *arg) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;

    if (arg->unk325 != 0)
        return;
    task = func_020372d0(func_0206c3d0, func_0206bef4, 0, 0xa00, 4, sizeof(UnkStruct_EffObj));
    if (task == func_02006abc(0))
        return;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    w->unk88 = arg;
    w->unk10 = arg->unk10;
    w->unk14 = arg->unk14;
    w->unk58 |= 0x2100;
    w->unk54 |= 2;
    w->unk54 |= 0x10;
    w->unk68 = func_ov000_02330670;
}
