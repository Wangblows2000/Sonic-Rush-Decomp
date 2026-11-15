#include <ov000_b/stageobj.h>

typedef struct {
    s32 unk0;
    u8 pad4[0x18 - 4];
    u32 unk18;
} UnkStruct_022c4560;

extern void *data_022c448c;
extern UnkStruct_022c4560 data_022c4560;
extern u8 data_ov000_023684c0[];
extern u8 data_ov000_0236a9f8[];

BOOL func_ov000_023270d8(UnkStruct_StageObj *a);
void func_ov000_02334264(UnkStruct_EffObj *w);
void func_ov000_023323e8(void);

void func_ov000_02332448(UnkStruct_StageObj *arg, s32 x, s32 y) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;

    if (func_ov000_023270d8(arg))
        return;
    task = func_020372d0(func_0206c3d0, func_0206bef4, 0, 0xa00, 4, sizeof(UnkStruct_EffObj));
    if (task == func_02006abc(0))
        return;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    func_02069f1c(w, data_ov000_023684c0, 0xffff, data_ov000_0236a9f8, data_022c448c);
    if (data_022c4560.unk18 & 0x20)
        w->spr1.unk2C |= 0x80008;
    if (data_022c4560.unk0 != 1) {
        func_0206a054(w, 0, 0x17);
        func_02069818(w, 5);
    } else {
        func_0206a054(w, 1, 0xc);
        func_02069818(w, 8);
    }
    func_02069918(w, 9);
    func_020698f0(w, 1);
    w->spr0.unk2C |= 0x400;
    w->spr1.unk2C |= 0x400;
    w->spr0.unk2C |= 0x200;
    w->spr1.unk2C |= 0x200;
    w->unk70 = func_ov000_023323e8;
    if (arg->unk5C & 1)
        w->unk5C |= 1;
    w->unk10 = arg->unk10;
    w->unk14 = arg->unk14;
    w->unk18 = arg->unk18;
    w->unk2E = 0;
    w->unk30 = 0;
    w->unk1 = arg->unk1 + arg->unk5;
    if (!(w->unk5C & 1)) {
        w->unk10 += x << 8;
    } else {
        w->unk10 -= x << 8;
        w->unk2E = -w->unk2E;
    }
    w->unk14 += y << 8;
    w->unk58 |= 0x100;
    w->unk5C |= 0x100;
    w->unk54 |= 2;
    func_ov000_02334264(w);
}
