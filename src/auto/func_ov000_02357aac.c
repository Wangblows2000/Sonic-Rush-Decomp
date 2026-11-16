/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_02033550();

void func_ov000_02357aac(s32 p0) {
    if (*(u32 *)((u8 *)p0 + 0xb60) != 1) {
        func_02033550(((u8 *)p0 + 0xa8c), 1);
        *(u32 *)((u8 *)p0 + 0xb60) = 1;
    }
    *(u32 *)((u8 *)p0 + 0xab8) = (*(u32 *)((u8 *)p0 + 0xab8) & ~4);
}

#pragma thumb reset
