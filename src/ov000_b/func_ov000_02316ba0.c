#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0x1C];
    u8 unk1C;
    u8 unk1D;
} UnkStruct_022c4560;

extern void *data_ov000_02369ba0;
extern UnkStruct_022c4560 data_022c4560;
extern u8 data_ov000_02366f78[], data_ov000_0236a254[];

void func_ov000_022fb9c8(void);
void func_ov000_023168a4(void);
void func_ov000_02316b5c(UnkStruct_StageObj *w);

UnkStruct_StageObj *func_ov000_02316ba0(s32 a, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_StageObj *w;
    vu16 zero;

    task = func_020372d0(func_0206c3d0, func_ov000_022fb9c8, 0, 0x500, 2, sizeof(UnkStruct_StageObj));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_StageObj));
    func_ov000_022fba50(w, a, b, c);
    func_02069f1c(w, data_ov000_02366f78, 0xffff, data_ov000_0236a254, data_ov000_02369ba0);
    if (data_022c4560.unk1C == 3 || (data_022c4560.unk1C == 8 && data_022c4560.unk1D == 0))
        func_0206a054(w, 0, 0x5b);
    else
        func_0206a054(w, 0, 2);
    w->spr0.unk46 = 0x14;
    w->spr1.unk46 = 0x14;
    func_0206e2b4(&w->coll0, 0, -0x10);
    w->coll0.unk1C = func_ov000_023168a4;
    w->coll0.unkA |= 0x800;
    w->unk58 |= 0x2100;
    if (w->unk1E0->unk2 == 0x44 || (u16)(w->unk1E0->unk2 + 0xffb7) <= 1)
        w->unk5C |= 2;
    if (w->unk1E0->unk2 != 0x46 && w->unk1E0->unk2 != 0x48 && w->unk1E0->unk2 != 0x4a && w->unk1E0->unk2 != 0x4c)
        w->unk5C &= ~1;
    if (w->unk1E0->unk2 <= 0x4c) {
        if (w->unk1E0->unk2 >= 0x4b)
            w->unk8 = 6;
        else if (w->unk1E0->unk2 >= 0x47)
            w->unk8 = 4;
        else if (w->unk1E0->unk2 >= 0x45)
            w->unk8 = 2;
    }
    func_ov000_02316b5c(w);
    return w;
}
