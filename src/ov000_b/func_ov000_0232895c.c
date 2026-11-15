#include <ov000_b/stageobj.h>

typedef struct {
    s16 a;
    s16 b;
} UnkStruct_02367b0c;

extern void *data_022c4488;
extern UnkStruct_02367b0c data_ov000_02367b0c;
extern u8 data_ov000_02367e38[], data_ov000_02367e58[], data_ov000_0236a990[];

void func_ov000_02328928(void);
void func_ov000_02328908(void);

void func_ov000_0232895c(s32 x, s16 y, s32 flip, s32 c) {
    UnkStruct_Task *task1;
    UnkStruct_Task *task2;
    UnkStruct_EffObj *w1;
    UnkStruct_EffObj *w2;
    UnkStruct_02367b0c off;
    vu16 zero1;
    vu16 zero2;

    off = data_ov000_02367b0c;
    task1 = func_020372d0(func_0206c3d0, func_0206bef4, 0, 0xa00, 4, sizeof(UnkStruct_EffObj));
    task2 = func_020372d0(func_0206c3d0, func_0206bef4, 0, 0xa00, 4, sizeof(UnkStruct_EffObj));
    if (task1 == func_02006abc(0))
        return;
    if (task2 == func_02006abc(0))
        return;
    w1 = task1->work;
    w2 = task2->work;
    zero1 = 0;
    MIi_CpuClear16(zero1, w1, sizeof(UnkStruct_EffObj));
    zero2 = 0;
    MIi_CpuClear16(zero2, w2, sizeof(UnkStruct_EffObj));
    w1->unkC = c;
    w2->unkC = c;
    func_02069f1c(w1, data_ov000_02367e38, 0x10, data_ov000_0236a990, data_022c4488);
    func_02069f1c(w2, data_ov000_02367e58, 0x10, data_ov000_0236a990, data_022c4488);
    func_0206a054(w1, 0xb, 0x52);
    func_0206a054(w2, 0xc, 0x52);
    func_02069918(w1, 3);
    func_02069918(w2, 3);
    func_020698f0(w1, 0);
    func_020698f0(w2, 0);
    func_02069818(w1, 0xb);
    func_02069818(w2, 0xc);
    if (flip) {
        w1->unk5C |= 1;
        w2->unk5C |= 1;
        off.a = -off.a;
        off.b = -off.b;
    }
    w1->spr0.unk4 = x + off.a;
    w1->spr0.unk6 = y;
    w1->spr0.unk2C |= 4;
    w1->spr1.unk2C |= 0x80000;
    w2->spr0.unk4 = x + off.b;
    w2->spr0.unk6 = y;
    w2->spr0.unk2C |= 4;
    w2->spr1.unk2C |= 0x80000;
    w1->unk58 |= 0x2100;
    w2->unk58 |= 0x2100;
    w1->unk54 |= 2;
    w2->unk54 |= 2;
    w1->unk70 = func_ov000_02328928;
    w2->unk70 = func_ov000_02328928;
    w1->unk68 = func_ov000_02328908;
    w2->unk68 = func_ov000_02328908;
}
