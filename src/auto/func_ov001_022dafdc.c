/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
s32 func_ov001_022da59c();
s32 MsgBox_IsClosed();
s32 func_ov001_022da41c();
s32 func_ov001_022da784();

void func_ov001_022dafdc(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    func_ov001_022da59c(*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10));
    s32 t1 = (*(u32 *)t0 + 0x1);
    *(u32 *)t0 = t1;
    s32 t2 = MsgBox_IsClosed(t1);
    if (t2 != 0) {
        *(u32 *)t0 = 0;
        *(u32 *)((u8 *)t0 + 0x4) = 0;
        func_ov001_022da41c(t0);
        func_ov001_022da784(t0);
    }
}

#pragma thumb reset
