/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_02033550();
s32 func_0203344c();

void func_ov000_02357a6c(s32 p0, s32 p1) {
    func_02033550(((u8 *)p0 + 0x804), ((u32)(((3 * p1) + 0x9) << 0x10) >> 0x10), 3);
    *(u32 *)((u8 *)p0 + 0x830) = (*(u32 *)((u8 *)p0 + 0x830) | 4);
    func_0203344c(((u8 *)p0 + 0x804), 0, 0);
}

#pragma thumb reset
