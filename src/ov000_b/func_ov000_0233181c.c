#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0x18];
    u32 unk18;
    u8 unk1C;
    u8 unk1D;
} UnkStruct_022c4560;

extern void *data_022c448c;
extern UnkStruct_022c4560 data_022c4560;
extern u8 data_ov000_02368460[];
extern u8 data_ov000_0236a9f8[];

s32 func_020027a4(s32 y, s32 x);
void func_020693f0(s16 *x, s16 *y, u8 a);
void func_ov000_0232e90c(UnkStruct_EffObj *w);

UnkStruct_EffObj *func_ov000_0233181c(UnkStruct_StageObj *arg, s16 x, s16 y, s32 unused) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;

    if (data_022c4560.unk1C >= 9 || data_022c4560.unk1D >= 2)
        return NULL;
    task = func_020372d0(func_0206c3d0, func_0206bef4, 0, 0xa00, 4, sizeof(UnkStruct_EffObj));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    func_02069f1c(w, data_ov000_02368460, 8, data_ov000_0236a9f8, data_022c448c);
    func_0206a054(w, 1, 0xc);
    func_02069818(w, 9);
    func_02069918(w, 9);
    if (data_022c4560.unk18 & 0x20)
        w->spr1.unk2C |= 0x80008;
    w->unk88 = arg;
    if (arg->unk5C & 1)
        x = -x;
    w->unk10 = arg->unk10;
    w->unk14 = arg->unk14;
    w->unk1 = func_020027a4(-0x800, ((UnkStruct_EffObj *)w->unk88)->unk2E) >> 8;
    w->unk1 += arg->unk5 + 0x40;
    func_020693f0(&x, &y, arg->unk1 + arg->unk5);
    w->unk2E = x;
    w->unk30 = y;
    w->unk10 += x << 8;
    w->unk14 += y << 8;
    w->unk58 |= 0x2100;
    w->unk5C |= 0x100;
    w->unk54 |= 2;
    func_ov000_0232e90c(w);
    return w;
}
