#include <nitro/types.h>

typedef struct {
    s32 unk0;
    s16 unk4;
    s16 unk6;
    s32 unk8;
    s32 unkC;
} UnkStruct_0235f850;

typedef struct {
    u8 pad0[0x10];
    void *work;
} UnkStruct_0235f850_Task;

void Task_DestroyRange(s32 a, s32 b);
UnkStruct_0235f850_Task *Task_Create(void (*f)(void), void (*g)(void), s32 b, s32 c, s32 d, s32 e);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void TimeSelectMenu_BrightnessFade_Main(void);

#pragma thumb on

void TimeSelectMenu_StartBrightnessFade(s32 x, s32 y, s32 a, s32 b) {
    UnkStruct_0235f850 *w;
    vu32 zero;

    Task_DestroyRange(0x4000, 0x4000);
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
    w = Task_Create(TimeSelectMenu_BrightnessFade_Main, NULL, 0, 0x4000, 0x1000, 0x10)->work;
    zero = 0;
    MIi_CpuClear32(zero, w, 0x10);
    w->unk0 = 0;
    w->unk4 = x;
    w->unk6 = y;
    w->unk8 = a;
    w->unkC = b;
}

#pragma thumb reset
