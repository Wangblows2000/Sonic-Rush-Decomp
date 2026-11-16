#include <ov000_b/stageobj.h>

extern void *data_022c4488;
extern u8 data_ov000_02367ec4[], data_ov000_0236aa28[];

void func_ov000_02334264(UnkStruct_EffObj *w);

UnkStruct_EffObj *func_ov000_02328fb8(UnkStruct_EffObj *arg, s32 x, s32 y) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;

    task = func_020372d0(func_0206c3d0, func_0206bef4, 0, 0xa00, 4, sizeof(UnkStruct_EffObj));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    func_02069f1c(w, data_ov000_02367ec4, 6, data_ov000_0236aa28, data_022c4488);
    func_0206a054(w, 0, 7);
    if ((arg->unk28 < 0 ? -arg->unk28 : arg->unk28) > 0x400 || arg->unk2A != 0)
        func_02069818(w, 1);
    func_02069918(w, 0x12);
    if (arg->unk5C & 1)
        w->unk5C |= 1;
    w->unk5C ^= 1;
    w->unk10 = arg->unk10 + (x << 8);
    w->unk14 = arg->unk14 + (y << 8);
    w->unk58 |= 0x2100;
    w->unk54 |= 2;
    func_ov000_02334264(w);
    return w;
}
