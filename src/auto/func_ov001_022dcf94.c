/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
s32 func_ov001_022da70c();
s32 MsgBox_IsClosed();
s32 func_ov001_022da33c();
s32 func_ov001_022da244();
s32 func_ov001_022da1cc();
s32 func_ov001_022d983c();
void func_ov001_022dd1ec();

void func_ov001_022dcf94(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    func_ov001_022da70c(*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10));
    s32 t1 = (*(u32 *)t0 + 0x1);
    *(u32 *)t0 = t1;
    s32 t2 = MsgBox_IsClosed(t1);
    if (t2 != 0) {
        *(u32 *)t0 = 0;
        *(u32 *)((u8 *)t0 + 0x4) = 0;
        func_ov001_022da33c(t0);
        func_ov001_022da244(t0, 1);
        func_ov001_022da1cc(t0, 1);
        *(u32 *)((u8 *)t0 + 0x48) = 1;
        func_ov001_022d983c(t0, 1);
        *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x8) = (u32)func_ov001_022dd1ec;
    }
}

#pragma thumb reset
