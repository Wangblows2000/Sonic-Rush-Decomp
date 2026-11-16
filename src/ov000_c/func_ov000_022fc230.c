#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
} UnkStruct_022fc230;

extern void *data_ov000_02369ba0;
extern u8 data_ov000_02366394[];
extern u8 data_ov000_02369dbc[];

void func_ov000_022fb9c8();
void func_ov000_022fc418();

UnkStruct_022fc230 *func_ov000_022fc230(UnkStruct_ObjParam *p, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_022fc230 *w;
    vu16 zero;
    s32 k;

    task = func_020372d0(func_0206c3d0, func_ov000_022fb9c8, 0, 0x500, 2, sizeof(UnkStruct_022fc230));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_022fc230));
    func_ov000_022fba50(w, (s32)p, b, c);
    func_02069f1c(w, data_ov000_02366394, 2, data_ov000_02369dbc, data_ov000_02369ba0);
    func_0206a054(w, 0, 0x27);
    func_02069918(w, 0x15);
    if (!(w->base.unk1E0->unk4 & 1))
        w->base.unk5C &= ~1;
    w->base.unk2E = 0xa00;
    if (w->base.unk5C & 1)
        w->base.unk2E = -w->base.unk2E;
    k = w->base.unk1E0->unk4 & 0xc;
    if (k != 0)
        w->base.unk2E = (w->base.unk2E * (((k >> 2) << 7) + 0x100)) >> 8;
    w->base.unk58 |= 0x110;
    w->base.unk5C |= 0x100;
    func_ov000_022fc418(w);
    return w;
}
