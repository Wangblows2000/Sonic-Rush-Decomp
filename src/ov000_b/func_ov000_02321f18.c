#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0x10];
} UnkStruct_0236a790;

extern void *data_ov000_02369ba0;
extern UnkStruct_0236a790 data_ov000_0236a790[];
extern u8 data_ov000_0236736c[], data_ov000_0236a700[];


void func_ov000_02321f18(s32 x, s32 y, s16 vx, s16 vy, u16 n) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;
    s32 i;

    task = func_020372d0(func_0206c3d0, func_0206bef4, 0, 0xa00, 4, sizeof(UnkStruct_EffObj));
    if (task == func_02006abc(0))
        return;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    func_02069f1c(w, data_ov000_0236736c, 0, data_ov000_0236a700, data_ov000_02369ba0);
    func_0206a054(w, 0, 5);
    i = n;
    func_02069818(w, (u16)i);
    func_02069eb0(w, 1, &data_ov000_0236a790[i + 0x13]);
    func_02069918(w, 0x16);
    func_020698f0(w, 3);
    if (Rand() & 1)
        w->unk5C |= 1;
    if (Rand() & 1)
        w->unk5C |= 2;
    w->unk10 = x;
    w->unk14 = y;
    w->unk2E = vx;
    w->unk30 = vy;
    w->unk40 = 0x2a;
    w->unk42 = 0xf00;
    w->unk58 |= 0x180;
    w->unk54 |= 2;
}
