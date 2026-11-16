#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x104];
} UnkStruct_0230c02c_Sub;

typedef struct {
    UnkStruct_EffObj base;
    UnkStruct_0230c02c_Sub sub;
} UnkStruct_0230c02c;

extern void *data_ov000_02369ba0;
extern void *data_022c448c;
extern UnkStruct_Task *data_ov000_02369f6c;
extern u8 data_ov000_02366a04[], data_ov000_02369f84[], data_ov000_02369fcc[], data_ov000_02369f7c[];
extern u8 data_ov000_02366a1c[], data_ov000_02369ffc[];

void func_ov000_0230bfdc(void);
void func_ov000_0230bff8(void);
void func_02069934(void *w, UnkStruct_0230c02c_Sub *s, void *a, s32 b, s32 c, void *d, void *e);
void func_02068358(UnkStruct_0230c02c_Sub *s, void *a, s32 b, s32 c);

UnkStruct_0230c02c *func_ov000_0230c02c(void) {
    UnkStruct_Task *task;
    UnkStruct_0230c02c *w;
    vu16 zero;

    if (data_ov000_02369f6c != NULL)
        return data_ov000_02369f6c->work;
    task = func_020372d0(func_0206c3d0, func_ov000_0230bfdc, 0, 0x3000, 4, sizeof(UnkStruct_0230c02c));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    data_ov000_02369f6c = task;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_0230c02c));
    func_02069f1c(w, data_ov000_02366a04, 0xffff, data_ov000_02369f84, data_022c448c);
    func_02069eb0(w, 0xffff, data_ov000_02369fcc);
    w->base.spr0.unk2C |= 0x80000;
    func_02069934(w, &w->sub, data_ov000_02366a1c, 0, 0, data_ov000_02369f7c, data_ov000_02369ba0);
    func_02068358(&w->sub, data_ov000_02369ffc, 0x80, 0x10);
    w->base.spr0.unk44 = 2;
    w->base.spr1.unk44 = 2;
    w->base.unk58 |= 0x2100;
    w->base.unk5C |= 0x20;
    w->base.unk5C |= 4;
    w->base.unk54 |= 0x10;
    w->base.unkC = 1;
    w->base.unk68 = func_ov000_0230bff8;
    return w;
}
