/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_02033550();
s32 func_0203344c();

void func_ov000_023582b0(s32 p0) {
    if (*(u32 *)((u8 *)p0 + 0xb50) != 0) {
        func_02033550(((u8 *)p0 + 0x96c), 0);
        func_0203344c(((u8 *)p0 + 0x96c), 0, 0);
        *(u32 *)((u8 *)p0 + 0xb50) = 0;
    }
}

#pragma thumb reset
