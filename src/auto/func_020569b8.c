/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
s32 func_02058350();
s32 Task_Destroy();

void func_020569b8(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    s32 t1 = func_02058350(*(u32 *)gTaskCurrent);
    *(u32 *)((u8 *)t0 + 0x4) = t1;
    if (*(u32 *)((u8 *)t0 + 0x4) == 0) {
        Task_Destroy(*(u32 *)gTaskCurrent);
        return;
    }
    *(u32 *)((u8 *)t0 + 0xc) = (*(u32 *)((u8 *)t0 + 0xc) + 0x1);
    s32 t2 = *(u32 *)t0;
    if (t2 != 0) {
        ((s32 (*)())t2)(t0, t2);
    }
}

#pragma thumb reset
