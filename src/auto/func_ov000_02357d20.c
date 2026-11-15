/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_02033550();
s32 func_0203344c();

void func_ov000_02357d20(s32 p0) {
    func_02033550(((u8 *)p0 + 0x60c), 3);
    func_0203344c(((u8 *)p0 + 0x60c), 0, 0);
    *(u32 *)((u8 *)p0 + 0x638) = (*(u32 *)((u8 *)p0 + 0x638) & ~4);
    *(u32 *)((u8 *)p0 + 0x638) = (*(u32 *)((u8 *)p0 + 0x638) | 1);
}

#pragma thumb reset
