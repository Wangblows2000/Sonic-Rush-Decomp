/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
s32 func_020613cc();
s32 func_02061484();
s32 Task_Destroy();

void func_02061700(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    *(u32 *)((u8 *)t0 + 0xc) = (*(u32 *)((u8 *)*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10) + 0xc) - 0x1);
    s32 t1 = *(u32 *)((u8 *)t0 + 0xc);
    if (t1 != 0) {
        func_020613cc(t0, t1);
        return;
    }
    func_02061484();
    Task_Destroy(*(u32 *)gTaskCurrent);
}

#pragma thumb reset
