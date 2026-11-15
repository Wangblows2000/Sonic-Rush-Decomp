/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_ov001_022da374();
s32 func_ov001_022da33c();
s32 func_02040ec0();
void func_ov001_022dbfdc();

void func_ov001_022dd534(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x10);
    func_ov001_022da374(*(u32 *)((u8 *)p0 + 0x10));
    func_ov001_022da33c(t0);
    func_02040ec0((t0 + 0xb44), 0x1c);
    func_02040ec0((t0 + 0xb70), 0x1d);
    *(u32 *)((u8 *)p0 + 0x8) = (u32)func_ov001_022dbfdc;
}

#pragma thumb reset
