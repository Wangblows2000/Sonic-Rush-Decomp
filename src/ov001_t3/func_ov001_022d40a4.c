#include <nitro/types.h>

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    u8 padc[0x5f4 - 0xc];
    s32 unk5f4;
    s32 unk5f8;
    s32 unk5fc;
    s32 unk600;
    void (*fn604)(void);
    void (*fn608)(void);
    void (*fn60c)(void);
    void (*fn610)(void);
} UnkWork_022d40a4;

typedef struct {
    u8 pad0[0x10];
    UnkWork_022d40a4 *work;
} UnkTask_022d40a4;

UnkTask_022d40a4 *Task_Create(void (*fn)(void), void (*dtor)(void), s32 b, s32 c, s32 d, s32 size);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void func_ov001_022d3cd0(void);
void func_ov001_022d3e14(void);
BOOL func_ov001_022d344c(void);
void func_ov001_022d3434(void);
void func_ov001_022d3410(void);
void func_ov001_022d3408(void);
void func_ov001_022d3400(void);
void func_ov001_022d33ec(void);

#pragma thumb on
UnkWork_022d40a4 *func_ov001_022d40a4(void) {
    UnkWork_022d40a4 *w;
    vu16 zero;

    w = Task_Create(func_ov001_022d3cd0, func_ov001_022d3e14, 0, 0x1000, 0x1000, sizeof(UnkWork_022d40a4))->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkWork_022d40a4));
    w->unk0 = 0;
    w->unk4 = 0;
    w->unk8 = 0;
    w->unk5f4 = 1;
    w->unk5f8 = 4;
    if (func_ov001_022d344c()) {
        w->unk5fc = 7;
    } else {
        w->unk5fc = 10;
    }
    w->unk600 = 0xd;
    w->fn604 = func_ov001_022d3434;
    w->fn608 = func_ov001_022d3410;
    if (func_ov001_022d344c()) {
        w->fn60c = func_ov001_022d3408;
    } else {
        w->fn60c = func_ov001_022d3400;
    }
    w->fn610 = func_ov001_022d33ec;
    return w;
}
#pragma thumb reset
