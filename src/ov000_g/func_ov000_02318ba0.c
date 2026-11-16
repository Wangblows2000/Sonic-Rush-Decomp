#include <ov000_b/stageobj.h>

typedef struct {
    s16 unk0;
    u16 unk2;
    s16 unk4;
    u8 pad6[2];
    void *unk8;
} UnkStruct_02318ba0;

typedef struct {
    u8 pad0[0x330];
    u32 unk330;
} UnkStruct_02318ba0_P;

extern s16 data_ov000_0236a2bc[];
extern UnkStruct_02318ba0_P *data_ov000_0236ac60;

void func_ov000_023189c4(void);
void func_ov000_02318b08(void);
void func_0204b7ec(void *p, s32 a, s32 b);
void func_ov000_0233e48c(s32 a);

void func_ov000_02318ba0(s16 a, u16 b) {
    UnkStruct_Task *task;
    UnkStruct_02318ba0 *w;
    vu16 zero;

    task = func_020372d0((void (*)(void))func_ov000_023189c4, func_ov000_02318b08, 0, 0xa00, 4, sizeof(UnkStruct_02318ba0));
    if (task == func_02006abc(0))
        return;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_02318ba0));
    w->unk0 = a;
    w->unk2 = b;
    func_0204b7ec(NULL, 0, 0x6e);
    w->unk8 = (void *)func_0203663c();
    if (data_ov000_0236a2bc[w->unk0] != 0x3fc)
        func_0204b7ec(w->unk8, 0, 0x6f);
    w->unk4 = 0;
    if (w->unk2 == 0)
        return;
    if (!(data_ov000_0236ac60->unk330 & 0x8000))
        return;
    func_ov000_0233e48c(3);
    data_ov000_0236ac60->unk330 |= 0x4000;
}
