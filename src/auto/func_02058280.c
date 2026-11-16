/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
s32 Task_Destroy();
s32 func_02057458();

void func_02058280(void) {
    s32 t0 = *(u32 *)gTaskCurrent;
    s32 t1 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    s32 t2 = (*(u32 *)((u8 *)*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10) + 0x4) & 0x80000000);
    if (t2 != 0) {
        Task_Destroy(t0, 0x80000000, t2);
        return;
    }
    *(u32 *)((u8 *)t1 + 0x8) = (*(u32 *)((u8 *)t1 + 0x8) + 0x1);
    s32 t3 = (*(u32 *)((u8 *)t1 + 0x4) & 1);
    if (t3 != 0) {
        *(u16 *)((u8 *)t1 + 0x20) = (*(u16 *)((u8 *)t1 + 0x20) + 0x1);
    }
    func_02057458(t1, t3);
    s32 t4 = *(u32 *)t1;
    if (t4 != 0) {
        ((s32 (*)())t4)(t1, t4);
    }
    s32 t5 = (*(u32 *)((u8 *)t1 + 0x4) & 0x80000000);
    if (t5 != 0) {
        Task_Destroy(*(u32 *)gTaskCurrent, t5);
    }
}

#pragma thumb reset
