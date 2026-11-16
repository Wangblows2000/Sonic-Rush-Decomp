/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
s32 func_ov001_022ddda8();
s32 Task_Destroy();
s32 func_ov001_022ddd7c();
extern u8 gSaveDataBusy[];
extern u8 gSystemFlags[];
s32 func_ov001_022d731c();
s32 func_ov001_022d717c();

void func_ov001_022d833c(void) {
    s32 t0 = *(u32 *)*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    s32 t1 = func_ov001_022ddda8(*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10));
    if (t1 != 0) {
        Task_Destroy(*(u32 *)gTaskCurrent);
        s32 t2 = func_ov001_022ddd7c();
        if (t2 != 0) {
            *(u32 *)gSaveDataBusy = 0;
            s32 t3 = (*(u32 *)gSystemFlags & ~0x10);
            *(u32 *)gSystemFlags = t3;
            func_ov001_022d731c(t0, gSystemFlags, t3);
            func_ov001_022d717c(t0, 1, 0);
            return;
        }
        *(u32 *)t0 = 2;
    }
}

#pragma thumb reset
