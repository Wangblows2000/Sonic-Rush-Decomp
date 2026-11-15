#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[8];
} UnkStruct_0236a6f0;

typedef struct {
    u8 pad0[0x10];
} UnkStruct_0236a790;

typedef struct {
    u8 pad0[0x20];
    u32 unk20;
} UnkStruct_022c4560;

extern void *data_ov000_02369ba0;
extern UnkStruct_022c4560 data_022c4560;
extern UnkStruct_Task *data_ov000_0236a660[];
extern UnkStruct_0236a6f0 data_ov000_0236a6f0[];
extern UnkStruct_0236a790 data_ov000_0236a790[];

void func_ov000_02322d14(void);
void func_ov000_02322d44(void);
void func_020332a8(UnkStruct_Spr *s, s16 a, s32 b, s32 c);

void *func_ov000_02322d7c(void *a, s32 b, s32 c, s32 d, u16 e) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;
    s32 i = e;

    if (data_ov000_0236a660[i] != NULL)
        return data_ov000_0236a660[i]->work;
    task = func_020372d0(func_0206c3d0, func_ov000_02322d14, 0, 0x3000, 4, sizeof(UnkStruct_EffObj));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    data_ov000_0236a660[i] = task;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    func_02069f1c(w, a, 0, &data_ov000_0236a6f0[d], data_ov000_02369ba0);
    func_02069eb0(w, b, &data_ov000_0236a790[e]);
    func_02069818(w, c);
    w->unk58 |= 0x2100;
    w->unk5C |= 0x20;
    w->unk5C |= 4;
    w->unk54 |= 0x10;
    w->unkC = 1;
    w->unk8 = (u16)e;
    w->unk68 = func_ov000_02322d44;
    w->spr0.unk2C |= 0x10;
    w->spr1.unk2C |= 0x10;
    func_020332a8(&w->spr0, (data_022c4560.unk20 & 0xf) << 4, 0, 0);
    func_020332a8(&w->spr1, (data_022c4560.unk20 & 0xf) << 4, 0, 0);
    return w;
}
