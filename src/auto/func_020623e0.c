/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

void func_02062340();
s32 Task_Create();

void func_020623e0(void) {
    Task_Create((u32)func_02062340, 0, 7, 0xf00a, 3, 0);
}

#pragma thumb reset
