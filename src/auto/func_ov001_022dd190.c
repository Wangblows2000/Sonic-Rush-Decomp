/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
s32 func_ov001_022da70c();
s32 func_02050018();
s32 func_ov001_022da41c();
s32 func_0205000c();
s32 func_02051838();
s32 func_ov001_022daaa8();
s32 func_ov001_022da33c();
void func_ov001_022dd1ec();

void func_ov001_022dd190(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    func_ov001_022da70c(*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10));
    s32 t1 = (*(u32 *)t0 + 0x1);
    *(u32 *)t0 = t1;
    s32 t2 = func_02050018(t1);
    if (t2 != 0) {
        *(u32 *)t0 = 0;
        *(u32 *)((u8 *)t0 + 0x4) = 0;
        func_ov001_022da41c(t0);
        s32 t3 = func_0205000c();
        if (t3 == 0) {
            func_02051838();
            func_ov001_022daaa8(t0);
            return;
        }
        func_ov001_022da33c(t0);
        *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x8) = (u32)func_ov001_022dd1ec;
    }
}

#pragma thumb reset
