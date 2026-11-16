/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
s32 func_02063030();
s32 func_02042958();
s32 Task_Destroy();
s32 func_02054dbc();
s32 func_02051660();
s32 func_02051598();
void func_020634e0();

void func_0206353c(void) {
    func_02063030(*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10));
    s32 t0 = func_02042958();
    if (t0 == 7) {
        Task_Destroy(*(u32 *)gTaskCurrent);
        func_02054dbc();
        return;
    }
    s32 t1 = func_02051660();
    if (t1 != 0) {
        func_02051598(0x40);
        *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x8) = (u32)func_020634e0;
    }
}

#pragma thumb reset
