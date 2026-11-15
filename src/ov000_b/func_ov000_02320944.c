#include <ov000_b/stageobj.h>
#include <ov000_b/res.h>

typedef struct {
    u8 pad0[0x10];
} UnkStruct_0236a790;

typedef struct {
    u8 pad0[0x18];
    u32 unk18;
} UnkStruct_022c4560;

extern void *data_ov000_02369ba0;
extern UnkStruct_022c4560 data_022c4560;
extern UnkStruct_Res *data_ov000_0236a6f0[];
extern UnkStruct_0236a790 data_ov000_0236a790[];
extern u8 data_ov000_02367324[], data_ov000_0236a718[];

void func_ov000_023208b0(void);

UnkStruct_EffObj *func_ov000_02320944(u32 a) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;
    u32 i;

    if (data_022c4560.unk18 & 0x20)
        return NULL;
    task = func_020372d0(func_0206c3d0, func_0206bef4, 0, 0xa00, 4, sizeof(UnkStruct_EffObj));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    w->unk10 = (a & 7) << 14;
    func_02069f1c(w, data_ov000_02367324, 0, data_ov000_0236a718, data_ov000_02369ba0);
    i = a & 3;
    func_02069eb0(w, Res_GetElemVal(data_ov000_0236a6f0[10], i), &data_ov000_0236a790[i + 0x14]);
    func_0206a054(w, 0, 0x64);
    func_02069818(w, (u16)i);
    if (a > 3) {
        w->spr0.unk2C |= 8;
        w->spr1.unk2C |= 8;
    }
    if (a & 8) {
        w->unk14 = 0xc400;
        w->unk2E = -0x40;
        func_020698f0(w, 3);
        func_02069918(w, 0x18);
    } else {
        w->unk14 = 0xe400;
        w->unk2E = -0x80;
        func_020698f0(w, 0);
        func_02069918(w, 5);
    }
    w->unk5C |= 0x20;
    w->unk5C |= 1;
    w->unk5C |= 0x80;
    w->unk58 |= 0x2100;
    w->unk54 |= 0x12;
    w->unk68 = func_ov000_023208b0;
    return w;
}
