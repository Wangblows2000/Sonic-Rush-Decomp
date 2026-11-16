/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_02050018();
s32 Touch_Disable();
s32 func_02054ac4();
extern u8 gTaskCurrent[];
void func_02054cbc();

void func_02054d3c(void) {
    s32 t0 = func_02050018();
    if (t0 != 0) {
        Touch_Disable();
        func_02054ac4();
        *(u32 *)*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10) = 0;
        *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x8) = (u32)func_02054cbc;
    }
}

#pragma thumb reset
