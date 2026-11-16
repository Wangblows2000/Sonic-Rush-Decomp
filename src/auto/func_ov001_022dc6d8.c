/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
s32 func_ov001_022da694();
s32 MsgBox_IsClosed();
s32 func_ov001_022da41c();
s32 MsgBox_GetResult();
s32 func_020511c0();
s32 func_ov001_022da9fc();
s32 func_ov001_022da33c();
void func_ov001_022dc734();

void func_ov001_022dc6d8(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    func_ov001_022da694(*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10));
    s32 t1 = (*(u32 *)t0 + 0x1);
    *(u32 *)t0 = t1;
    s32 t2 = MsgBox_IsClosed(t1);
    if (t2 != 0) {
        *(u32 *)t0 = 0;
        *(u32 *)((u8 *)t0 + 0x4) = 0;
        func_ov001_022da41c(t0);
        s32 t3 = MsgBox_GetResult();
        if (t3 == 0) {
            func_020511c0();
            func_ov001_022da9fc(t0);
            return;
        }
        func_ov001_022da33c(t0);
        *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x8) = (u32)func_ov001_022dc734;
    }
}

#pragma thumb reset
