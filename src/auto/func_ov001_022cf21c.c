/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
s32 SysSound_PlaySE();
s32 func_0205004c();
s32 func_02050018();
s32 SysSound_Release();
s32 Task_Destroy();
s32 func_02059b5c();
s32 func_0205a4ec();
s32 func_ov001_022cf324();

void func_ov001_022cf21c(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    if (*(u32 *)*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10) == 0xa) {
        SysSound_PlaySE(0x15);
    }
    if (*(u32 *)t0 == 0x28) {
        SysSound_PlaySE(0x16);
    }
    s32 t1 = *(u32 *)t0;
    if (t1 == 0xf0) {
        func_0205004c(t1);
    }
    s32 t2 = (*(u32 *)t0 + 0x1);
    *(u32 *)t0 = t2;
    s32 t3 = func_02050018(t2);
    if (t3 != 0) {
        *(volatile u32 *)0x4000000 = ((*(volatile u32 *)0x4000000 & -7937) | 0x1000);
        s32 t4 = ((*(volatile u32 *)0x4001000 & -7937) | 0x1000);
        *(volatile u32 *)0x4001000 = t4;
        s32 t5 = *(u32 *)((u8 *)t0 + 0x4);
        SysSound_Release(-7937, 0x1000, 0x4001000, t4);
        Task_Destroy(*(u32 *)gTaskCurrent);
        func_02059b5c();
        s32 t6 = func_0205a4ec(0);
        if (t6 != 0) {
            func_ov001_022cf324(t5);
        }
    }
}

#pragma thumb reset
