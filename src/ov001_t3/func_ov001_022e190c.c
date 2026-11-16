#include <nitro/types.h>

typedef struct UnkTask_022e190c UnkTask_022e190c;

typedef struct {
    s32 unk0;
    u8 pad4[8];
    s32 unkc;
    s32 unk10;
    u8 pad14[0xd8 - 0x14];
    s32 unkd8;
    s32 unkdc;
    u8 pade0[0xea8 - 0xe0];
    UnkTask_022e190c *task;
    u8 padeac[0xec0 - 0xeac];
} UnkWork_022e190c;

struct UnkTask_022e190c {
    u8 pad0[0x10];
    UnkWork_022e190c *work;
};

extern UnkWork_022e190c *data_ov001_0231f05c;
extern s32 data_ov001_0231c3ec;
extern s32 data_ov001_0231c3f0;

void func_ov001_022e1900(void);
UnkTask_022e190c *Task_Create(void (*fn)(void), void (*dtor)(void), s32 b, s32 c, s32 d, s32 size);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void func_ov001_022e129c(void);
void func_ov001_022e0ee0(void);
void func_ov001_022e0784(UnkWork_022e190c *w);
void func_ov001_022e06e4(UnkWork_022e190c *w);
void func_ov001_022e17fc(void);
void func_ov001_022e17c8(UnkWork_022e190c *w);
void func_ov001_022e153c(UnkWork_022e190c *w);
void func_ov001_022e0e98(void);

#pragma thumb on
void func_ov001_022e190c(s32 a, s32 b, s32 c) {
    vu32 zero;
    UnkTask_022e190c *t;
    UnkWork_022e190c *w;

    func_ov001_022e1900();
    t = Task_Create(func_ov001_022e129c, func_ov001_022e0ee0, 0, 0x8000, 0, sizeof(UnkWork_022e190c));
    w = t->work;
    data_ov001_0231f05c = w;
    zero = 0;
    MIi_CpuClear32(zero, w, sizeof(UnkWork_022e190c));
    data_ov001_0231f05c->unk0 = 0;
    data_ov001_0231f05c->unkc = b + 1;
    data_ov001_0231f05c->task = t;
    data_ov001_0231f05c->unkd8 = 0;
    data_ov001_0231f05c->unkdc = 0;
    data_ov001_0231f05c->unk10 = c;
    data_ov001_0231c3ec = a;
    data_ov001_0231c3f0 = 0;
    func_ov001_022e0784(data_ov001_0231f05c);
    func_ov001_022e06e4(data_ov001_0231f05c);
    func_ov001_022e17fc();
    func_ov001_022e17c8(data_ov001_0231f05c);
    func_ov001_022e153c(data_ov001_0231f05c);
    func_ov001_022e0e98();
}
#pragma thumb reset
