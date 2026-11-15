/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_02033550();
s32 func_0203344c();

void func_ov000_02357c8c(s32 p0) {
    func_02033550(((u8 *)p0 + 0x654), 3);
    func_0203344c(((u8 *)p0 + 0x654), 0, 0);
    *(u32 *)((u8 *)p0 + 0x680) = (*(u32 *)((u8 *)p0 + 0x680) & ~4);
    *(u32 *)((u8 *)p0 + 0x680) = (*(u32 *)((u8 *)p0 + 0x680) | 1);
    *(u32 *)((u8 *)p0 + 0x680) = (*(u32 *)((u8 *)p0 + 0x680) | 0x80);
}

#pragma thumb reset
