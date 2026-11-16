/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_02033550();
void func_ov001_022d1a24();
s32 func_ov001_022d2234();

void func_ov001_022d1bec(s32 p0) {
    func_02033550(((u8 *)p0 + 0xbe4), 0x12);
    func_02033550(((u8 *)p0 + 0xc2c), 0x18);
    func_ov001_022d2234(p0, (u32)func_ov001_022d1a24);
}

#pragma thumb reset
