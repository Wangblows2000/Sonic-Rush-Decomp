#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0x1C];
    u8 unk1C;
    u8 unk1D;
} UnkStruct_022c4560;

extern void *data_ov000_02369ba0;
extern UnkStruct_022c4560 data_022c4560;
extern u8 data_ov000_02366cac[], data_ov000_0236a0cc[];

void func_ov000_022fb9c8(void);
void func_ov000_023102b4(void);
BOOL func_ov000_02345758(s32 x, s32 y);
void func_ov000_023106b8(UnkStruct_StageObj *w);
void func_ov000_023107c4(UnkStruct_StageObj *w);

UnkStruct_StageObj *func_ov000_023107f4(s32 a, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_StageObj *w;
    vu16 zero;
    BOOL flag;

    task = func_020372d0(func_0206c3d0, func_ov000_022fb9c8, 0, 0x500, 2, sizeof(UnkStruct_StageObj));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_StageObj));
    func_ov000_022fba50(w, a, b, c);
    func_02069f1c(w, data_ov000_02366cac, 0x10, data_ov000_0236a0cc, data_ov000_02369ba0);
    func_0206a054(w, 0, 2);
    w->spr0.unk46 = 0x14;
    w->spr1.unk46 = 0x14;
    func_0206e2b4(&w->coll0, 0, -0x10);
    w->coll0.unk1C = func_ov000_023102b4;
    w->unk58 |= 0x2100;
    if (func_ov000_02345758(w->unk10, w->unk14))
        w->unk5C |= 2;
    flag = FALSE;
    if (data_022c4560.unk1C >= 7 && data_022c4560.unk1C < 9 && data_022c4560.unk1D < 2)
        flag = TRUE;
    if (flag)
        func_ov000_023106b8(w);
    else
        func_ov000_023107c4(w);
    return w;
}
