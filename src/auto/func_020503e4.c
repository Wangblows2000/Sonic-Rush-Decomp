/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_02051660();
extern u8 gTaskCurrent[];
s32 Task_Destroy();

void func_020503e4(void) {
    s32 t0 = func_02051660();
    if (t0 != 0) {
        s32 t1 = *(u32 *)*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
        Task_Destroy(*(u32 *)gTaskCurrent, *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10));
        ((s32 (*)())t1)();
    }
}

#pragma thumb reset
