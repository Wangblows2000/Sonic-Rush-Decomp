/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_02033550();
s32 func_0203344c();

void func_ov001_022ea6e4(s32 p0) {
    func_02033550(((u8 *)p0 + 0x858), 2);
    *(u32 *)((u8 *)p0 + 0xb38) = 2;
    func_0203344c(((u8 *)p0 + 0x858), 0, 0);
    *(u32 *)((u8 *)p0 + 0x884) = (*(u32 *)((u8 *)p0 + 0x884) | 4);
    *(u32 *)((u8 *)p0 + 0x884) = (*(u32 *)((u8 *)p0 + 0x884) | 0x80);
    *(u32 *)((u8 *)p0 + 0x884) = (*(u32 *)((u8 *)p0 + 0x884) | 1);
}

#pragma thumb reset
