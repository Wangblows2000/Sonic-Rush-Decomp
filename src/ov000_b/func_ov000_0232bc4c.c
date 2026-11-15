#include <ov000_b/stageobj.h>

extern void *data_022c448c;
extern u8 data_ov000_02367ff8[], data_ov000_0236aa18[], data_ov000_02368010[], data_ov000_02368028[];

void func_ov000_0232bc28(UnkStruct_EffObj *w);

void func_ov000_0232bc4c(s32 type, s32 x, s32 y, s16 vx, s16 vy, s16 c) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;

    task = func_020372d0(func_0206c3d0, func_0206bef4, 0, 0xa00, 4, sizeof(UnkStruct_EffObj));
    if (task == func_02006abc(0))
        return;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    switch (type) {
    default:
        func_02069f1c(w, data_ov000_02367ff8, 9, data_ov000_0236aa18, data_022c448c);
        func_0206a054(w, 0, 2);
        func_02069818(w, 6);
        break;
    case 1:
        func_02069f1c(w, data_ov000_02368010, 0xa, data_ov000_0236aa18, data_022c448c);
        w->spr0.unk44 = 1;
        w->spr1.unk44 = 1;
        w->spr0.unk2C |= 0x10;
        w->spr1.unk2C |= 0x10;
        func_02069818(w, 8);
        break;
    case 2:
        func_02069f1c(w, data_ov000_02368028, 0xc, data_ov000_0236aa18, data_022c448c);
        func_0206a054(w, 0, 2);
        func_02069818(w, 0xa);
        break;
    }
    func_02069918(w, 9);
    func_020698f0(w, 0);
    w->unk5C |= 0x80;
    w->unk54 |= 0x10;
    w->unk10 = (x + 0x80) << 8;
    w->unk14 = (y + 0xc0) << 8;
    w->unk58 |= 0x180;
    w->unk54 |= 2;
    w->unk5C |= 4;
    w->unk2E = vx;
    w->unk30 = vy;
    w->unkC = c;
    w->unk40 = 0x54;
    w->unk42 = 0x1e00;
    func_ov000_0232bc28(w);
}
