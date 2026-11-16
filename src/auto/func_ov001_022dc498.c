/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 data_022b4574[];
s32 func_ov001_022da694();
s32 func_02050018();
s32 func_ov001_022da33c();
s32 func_ov001_022da244();
s32 func_ov001_022da1cc();
s32 func_ov001_022d983c();
void func_ov001_022dc734();

void func_ov001_022dc498(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10);
    func_ov001_022da694(*(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10));
    s32 t1 = (*(u32 *)t0 + 0x1);
    *(u32 *)t0 = t1;
    s32 t2 = func_02050018(t1);
    if (t2 != 0) {
        *(u32 *)t0 = 0;
        *(u32 *)((u8 *)t0 + 0x4) = 0;
        func_ov001_022da33c(t0);
        func_ov001_022da244(t0, 1);
        func_ov001_022da1cc(t0, 1);
        *(u32 *)((u8 *)t0 + 0x48) = 1;
        func_ov001_022d983c(t0, 1);
        *(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x8) = (u32)func_ov001_022dc734;
    }
}

#pragma thumb reset
