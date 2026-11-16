#include <nitro/types.h>

void Task_Create(void (*f)(void), s32 a, s32 b, s32 c, s32 d, s32 e);
void func_ov001_022e9dac(void);

#pragma thumb on
void func_ov001_022e9dc8(void) {
    Task_Create(func_ov001_022e9dac, 0, 0, 0x2000, 0x1000, 0);
}
#pragma thumb reset
