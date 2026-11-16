/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
s32 Task_Destroy();

void func_02061780(void) {
    s32 t0 = *(u32 *)*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    if (t0 != 0) {
        ((s32 (*)())t0)(t0);
    }
    Task_Destroy(*(u32 *)gTaskCurrent);
}

#pragma thumb reset
