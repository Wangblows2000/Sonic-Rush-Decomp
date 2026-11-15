/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_02033550();

void func_ov000_02357b24(s32 p0) {
    func_02033550(((u8 *)p0 + 0x6e4), 7);
    *(u32 *)((u8 *)p0 + 0x710) = (*(u32 *)((u8 *)p0 + 0x710) & ~4);
}

#pragma thumb reset
