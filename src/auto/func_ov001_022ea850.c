/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_02033550();

void func_ov001_022ea850(s32 p0) {
    if (*(u32 *)((u8 *)p0 + 0xb30) != 0) {
        func_02033550(((u8 *)p0 + 0x7c8), 0);
        *(u32 *)((u8 *)p0 + 0xb30) = 0;
    }
    *(u32 *)((u8 *)p0 + 0x7f4) = (*(u32 *)((u8 *)p0 + 0x7f4) & ~4);
    *(u32 *)((u8 *)p0 + 0x7f4) = (*(u32 *)((u8 *)p0 + 0x7f4) & ~1);
}

#pragma thumb reset
