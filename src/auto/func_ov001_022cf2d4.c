/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

void func_ov001_022cf110();
s32 Task_Create();

void func_ov001_022cf2d4(s32 p0) {
    s32 t0 = Task_Create((u32)func_ov001_022cf110, 0, 0, 0x1000, 0, 8);
    s32 t1 = *(u32 *)((u8 *)t0 + 0x10);
    *(u32 *)t1 = 0;
    *(u32 *)((u8 *)t1 + 0x4) = p0;
    *(volatile u32 *)0x4000000 = (*(volatile u32 *)0x4000000 & -7937);
    *(volatile u32 *)0x4001000 = (*(volatile u32 *)0x4001000 & -7937);
}

#pragma thumb reset
