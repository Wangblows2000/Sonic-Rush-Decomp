/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
s32 func_02042958();
s32 func_02054dbc();
s32 func_020514ec();
s32 Task_Destroy();

void func_ov000_02361300(void) {
    s32 t0 = *(u32 *)*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    s32 t1 = func_02042958(*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10));
    if (t1 == 7) {
        func_02054dbc();
        return;
    }
    s32 t2 = func_020514ec();
    if (t2 != 0) {
        *(u32 *)((u8 *)t0 + 0x20) = 1;
        Task_Destroy(*(u32 *)gTaskCurrent);
    }
}

#pragma thumb reset
