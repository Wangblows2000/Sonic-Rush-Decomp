/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

void func_0205fb68();
s32 func_020073b8();

void func_0205fcac(u8 *p0) {
    *(u8 *)((u8 *)p0 + 0x14) = 0;
    *(u16 *)((u8 *)p0 + 0xec) = 0;
    *(u16 *)((u8 *)p0 + 0xee) = *(u16 *)((u8 *)p0 + 0x20);
    *(u16 *)((u8 *)p0 + 0xf0) = 0;
    *(u16 *)((u8 *)p0 + 0xf2) = 0;
    func_020073b8(((u8 *)p0 + 0x24), ((s32)((*(u8 *)((u8 *)p0 + 0x16) - 0x1) << 0x10) >> 0x10), 5, (u32)func_0205fb68, *(u8 *)((u8 *)p0 + 0x14));
}

#pragma thumb reset
