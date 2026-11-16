/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
s32 func_02063030();
s32 func_02042958();
s32 Task_Destroy();
s32 func_02054dbc();
s32 func_020514ec();
s32 func_02051384();
extern u8 data_022c4500[];
s32 func_0204319c();
s32 func_0204331c();
void func_0206340c();

void func_02063480(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    func_02063030(*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10));
    s32 t1 = func_02042958();
    if (t1 == 7) {
        Task_Destroy(*(u32 *)gTaskCurrent);
        func_02054dbc();
        return;
    }
    s32 t2 = func_020514ec();
    if (t2 != 0) {
        func_02051384();
        func_0204319c(data_022c4500);
        func_0204331c(1);
        *(u32 *)((u8 *)t0 + 0xc) = 0;
        *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x8) = (u32)func_0206340c;
    }
}

#pragma thumb reset
