#include <ov000_b/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[2];
    u16 unk20E;
} UnkStruct_0231ba3c;

typedef struct {
    u32 unk0;
    u8 pad4[0x62 - 4];
    u16 unk62;
    u32 unk64;
    u8 pad68[0xC6 - 0x68];
    u16 unkC6;
} UnkStruct_0236aad8;

extern UnkStruct_0236aad8 data_ov000_0236aad8;

void func_ov000_0231ba30(void);
void func_ov000_0231b834(UnkStruct_0231ba3c *w);
void func_ov000_0231b9f0(UnkStruct_0231ba3c *w);

UnkStruct_0231ba3c *func_ov000_0231ba3c(UnkStruct_ObjParam *p, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_0231ba3c *w;
    vu16 zero;

    if (p->unk4 & 4) {
        data_ov000_0236aad8.unk62 = data_ov000_0236aad8.unkC6 = (u8)p->unk6 * 10000 + (u8)p->unk7 * 100 + p->unk8;
        data_ov000_0236aad8.unk0 |= 0x1000000;
        data_ov000_0236aad8.unk64 |= 0x1000000;
        p->unk0 = 0xff;
        return NULL;
    }
    task = Task_Create(StageObj_Main, func_ov000_0231ba30, 0, 0x500, 2, sizeof(UnkStruct_0231ba3c));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_0231ba3c));
    func_ov000_022fba50(w, (s32)p, b, c);
    w->base.unk58 |= 0x2100;
    w->base.unk5C |= 0x20;
    w->unk20E = (u8)p->unk6 * 10000 + (u8)p->unk7 * 100 + p->unk8;
    if (p->unk4 & 0x10)
        func_ov000_0231b834(w);
    else
        func_ov000_0231b9f0(w);
    return w;
}
