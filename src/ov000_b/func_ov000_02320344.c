#include <ov000_b/stageobj.h>
#include <ov000_b/res.h>

typedef struct {
    u8 pad0[0x20];
    u32 unk20;
} UnkStruct_022c4560;

extern void *data_ov000_02369ba0;
extern UnkStruct_022c4560 data_022c4560;
extern UnkStruct_Res *data_ov000_0236a6f0[];
extern u8 data_ov000_023672d8[], data_ov000_0236a740[];

s32 func_02039c18(s32 a, s32 b);
void func_0206bd24(void *w, UnkStruct_Spr *a, UnkStruct_Spr *b);
void func_ov000_023202f8(void);


UnkStruct_EffObj *func_ov000_02320344(UnkStruct_StageObj *arg, u16 idx) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;

    task = func_020372d0(func_0206c3d0, func_0206bef4, 0, 0x4ff, 4, sizeof(UnkStruct_EffObj));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    func_02069f1c(w, data_ov000_023672d8, 0, data_ov000_0236a740, data_ov000_02369ba0);
    w->spr0.unk34 = func_02039c18(0, Res_GetElemVal(data_ov000_0236a6f0[20], idx));
    w->spr1.unk34 = func_02039c18(1, Res_GetElemVal(data_ov000_0236a6f0[20], idx));
    func_02069918(w, 0x16);
    func_02069818(w, idx);
    if (idx >= 0xd) {
        func_0206a054(w, 0xd, 0x62);
        idx -= 0xd;
        w->unk8 = idx + 0x10;
        if (!(data_022c4560.unk20 & 1))
            func_0206bd24(w, &w->spr0, &w->spr1);
    } else {
        func_0206a054(w, 0, 0x61);
        w->unk8 = idx;
        if (data_022c4560.unk20 & 1)
            func_0206bd24(w, &w->spr0, &w->spr1);
    }
    w->unk10 = arg->unk10;
    w->unk14 = arg->unk14 + (idx << 11) - 0x3000;
    w->unk88 = arg;
    w->unk5C |= 1;
    w->unk58 |= 0x2100;
    w->unk54 |= 0x12;
    w->unk68 = func_ov000_023202f8;
    return w;
}
