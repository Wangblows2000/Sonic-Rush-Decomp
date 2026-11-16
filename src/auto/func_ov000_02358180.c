/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_02033550();
s32 func_0203438c();
extern u8 data_02087d04[];

void func_ov000_02358180(u8 *p0) {
    if (*(u32 *)((u8 *)p0 + 0xb1c) != 1) {
        func_02033550(((u8 *)p0 + 0x5c4), 1);
        *(u32 *)((u8 *)p0 + 0xb1c) = 1;
        func_0203438c(((s32)((u8 *)p0 + 0x344) + ((*(u32 *)((u8 *)p0 + 0x14) + 0x3) << 6)), ((u8 *)p0 + 0x344));
        *(u16 *)((u8 *)data_02087d04 + 0x8) = 0;
        *(u16 *)((u8 *)data_02087d04 + 0xa) = 0x20;
    }
    *(u32 *)((u8 *)p0 + 0x5f0) = (*(u32 *)((u8 *)p0 + 0x5f0) & ~4);
}

#pragma thumb reset
