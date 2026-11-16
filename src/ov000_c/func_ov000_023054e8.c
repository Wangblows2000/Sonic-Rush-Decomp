#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    UnkStruct_Blk blk;
    u8 pad2BC[0xA2C - 0x2BC];
} UnkStruct_023054e8;

extern void *data_ov000_02369ba0;
extern u8 data_ov000_023667e4[], data_ov000_02369e9c[], data_ov000_02369e94[], data_ov000_023667fc[];

void func_ov000_022fb9c8(void);
void func_ov000_02305450(void);

UnkStruct_023054e8 *func_ov000_023054e8(UnkStruct_ObjParam *p, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_023054e8 *w;
    vu16 zero;

    task = func_020372d0(func_0206c3d0, func_ov000_022fb9c8, 0, 0x500, 2, sizeof(UnkStruct_023054e8));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_023054e8));
    func_ov000_022fba50(w, (s32)p, b, c);
    func_02069f1c(w, data_ov000_023667e4, 0x10, data_ov000_02369e9c, data_ov000_02369ba0);
    func_0206a054(w, 0, 0x36);
    func_02069918(w, 0x14);
    func_02069818(w, 3);
    w->base.spr0.unk2C |= 0x80000;
    func_02069a34(w, &w->blk, data_ov000_023667fc, 0, 2, 0xb, data_ov000_02369e94, data_ov000_02369ba0);
    w->base.unk5C |= 0x200;
    Obj_SetVec(&w->base.unk128->unk18, 0x34cc, 0x34cc, 0x34cc);
    w->base.unk70 = func_ov000_02305450;
    w->base.unk58 |= 0x2100;
    w->base.unk54 |= 0x10;
    return w;
}
