/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_02033550();

void func_ov000_02357b74(s32 p0) {
    func_02033550(((u8 *)p0 + 0x9fc), 0x1d);
    *(u32 *)((u8 *)p0 + 0xa28) = (*(u32 *)((u8 *)p0 + 0xa28) | 4);
}

#pragma thumb reset
