/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
s32 func_0204f038();
s32 Task_Destroy();

void func_0204f9f8(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    func_0204f038(*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10), (7 - *(u32 *)*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10)), 8, 0x1000);
    *(u32 *)t0 = (*(u32 *)t0 + 0x1);
    if ((u32)*(u32 *)t0 >= (u32)8) {
        Task_Destroy(*(u32 *)gTaskCurrent);
    }
}

#pragma thumb reset
