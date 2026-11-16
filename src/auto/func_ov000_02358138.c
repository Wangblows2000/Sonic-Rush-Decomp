/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_02033550();
extern u8 data_02087d04[];

void func_ov000_02358138(s32 p0) {
    if (*(u32 *)((u8 *)p0 + 0xb1c) != 2) {
        func_02033550(((u8 *)p0 + 0x5c4), 2);
        *(u32 *)((u8 *)p0 + 0xb1c) = 2;
    }
    *(u32 *)((u8 *)p0 + 0x5f0) = (*(u32 *)((u8 *)p0 + 0x5f0) & ~4);
    *(u16 *)((u8 *)data_02087d04 + 0xa) = 0xfff8;
}

#pragma thumb reset
