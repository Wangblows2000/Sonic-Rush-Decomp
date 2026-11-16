#include <nitro/types.h>

typedef struct {
    s32 unk0;
    s16 unk4;
    s16 unk6;
    s32 unk8;
    s32 unkC;
} UnkStruct_02356694;

typedef struct {
    u8 pad0[0x10];
    void *work;
} UnkStruct_02356694_Task;

void func_02037038(s32 a, s32 b);
UnkStruct_02356694_Task *func_020372d0(void (*f)(void), void (*g)(void), s32 b, s32 c, s32 d, s32 e);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void func_ov000_023565bc(void);

#pragma thumb on

void func_ov000_02356694(s32 x, s32 y, s32 a, s32 b) {
    UnkStruct_02356694 *w;
    vu32 zero;

    func_02037038(0x3000, 0x3000);
    if (x < -16) {
        x = -16;
    }
    if (x > 16) {
        x = 16;
    }
    if (y < -16) {
        y = -16;
    }
    if (y > 16) {
        y = 16;
    }
    w = func_020372d0(func_ov000_023565bc, NULL, 0, 0x3000, 0x1000, 0x10)->work;
    zero = 0;
    MIi_CpuClear32(zero, w, 0x10);
    w->unk0 = 0;
    w->unk4 = x;
    w->unk6 = y;
    w->unk8 = a;
    w->unkC = b;
}

#pragma thumb reset
