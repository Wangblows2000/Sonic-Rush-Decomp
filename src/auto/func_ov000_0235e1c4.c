/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

void func_ov000_0235e190();
s32 Task_Create();
s32 func_02050078();
s32 Touch_Enable();

void func_ov000_0235e1c4(void) {
    Task_Create((u32)func_ov000_0235e190, 0, 0, 0, 0, 0);
    func_02050078(0xc, 3, 2, 1, 0, 0, 0xe, 0x200010);
    *(volatile u16 *)0x5000000 = 0;
    *(volatile u16 *)0x5000400 = 0;
    *(volatile u32 *)0x4000000 = (*(volatile u32 *)0x4000000 & -7937);
    s32 t0 = ((*(volatile u32 *)0x4001000 & -7937) | 0x1000);
    *(volatile u32 *)0x4001000 = t0;
    Touch_Enable(0x1000, 0x4001000, t0);
}

#pragma thumb reset
