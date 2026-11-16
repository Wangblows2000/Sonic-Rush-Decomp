/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
s32 Task_DestroyByTag();
extern u8 data_ov001_0231bcf8[];

void func_ov001_022d8c44(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    *(u32 *)((u8 *)t0 + 0x4) = (*(u32 *)((u8 *)*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10) + 0x4) + 0x1);
    s32 t1 = *(u32 *)t0;
    if (t1 != 9) {
        Task_DestroyByTag(0x1000, t0);
        ((s32 (*)())*(u32 *)((u8 *)data_ov001_0231bcf8 + (t1 << 2)))(*(u32 *)((u8 *)data_ov001_0231bcf8 + (t1 << 2)), (t1 << 2));
    }
}

#pragma thumb reset
