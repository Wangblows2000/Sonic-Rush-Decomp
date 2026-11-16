#include <nitro/types.h>

typedef struct {
    u32 unk0;
    u32 unk4;
    u32 unk8;
} UnkWork_020508a8;

typedef struct {
    u32 unk0;
    u32 unk4;
    void (*fn)(void);
    u32 unkc;
    UnkWork_020508a8 *work;
} UnkTask_020508a8;

extern UnkTask_020508a8 *data_022c4638;
extern UnkTask_020508a8 *data_022c4634;
UnkTask_020508a8 *Task_Create(void (*fn)(void), s32 a, s32 b, s32 c, s32 d, s32 e);
void func_02050430(void);

#pragma thumb on
void func_020508a8(s32 a, s32 b) {
    u32 v = (a + 1) | ((b + 1) << 8);
    if (data_022c4638 == NULL) {
        data_022c4634 = Task_Create(func_02050430, 0, 0, 0x1000, 0, 4);
        data_022c4634->work->unk0 = v;
    } else {
        data_022c4638->work->unk8 = v;
        data_022c4638->fn = func_02050430;
    }
}
#pragma thumb reset
