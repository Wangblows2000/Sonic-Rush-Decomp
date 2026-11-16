/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_ov001_022da374();
s32 func_ov001_022da33c();
s32 func_02040ec0();
s32 func_02040e80();
s32 func_ov001_022d6dac();
void func_ov001_022dbdb0();
extern u8 data_022b4574[];

void func_ov001_022da9a8(s32 p0) {
    func_ov001_022da374();
    func_ov001_022da33c(p0);
    func_02040ec0(((u8 *)p0 + 0xb44), 0x1c);
    func_02040ec0(((u8 *)p0 + 0xb70), 0x1d);
    func_02040e80(((u8 *)p0 + 0xb44));
    func_02040e80(((u8 *)p0 + 0xb70));
    func_ov001_022d6dac();
    *(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x8) = (u32)func_ov001_022dbdb0;
}

#pragma thumb reset
