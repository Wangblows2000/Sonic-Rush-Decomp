#include <nitro/types.h>

typedef struct { u8 pad[0x10]; s32 *work; } UnkStruct_022c8994;
UnkStruct_022c8994 *Task_Create(void (*f)(void), s32 a, s32 b, s32 c, s32 d, s32 e);
void OptionsBgScroll_Main(void);

#pragma thumb on
void Options_CreateBgScrollTask(void) {
    s32 *w = Task_Create(OptionsBgScroll_Main, 0, 0, 0x4000, 0x1000, 8)->work;
    w[0] = 0;
    w[1] = 0;
}
#pragma thumb reset
