/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_0203473c();
s32 func_0203438c();

void func_ov000_0235ed5c(s32 p0) {
    func_0203473c(((u8 *)p0 + 0x57c), *(u32 *)((u8 *)p0 + 0xe24), 0x38, 0, 3, *(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0xe24) + 0x18), *(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0xe24) + 0x1a));
    func_0203438c(((u8 *)p0 + 0x57c));
    func_0203473c(((u8 *)p0 + 0x5bc), *(u32 *)((u8 *)p0 + 0xe24), 0x38, 1, 3, *(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0xe24) + 0x18), *(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0xe24) + 0x1a));
    func_0203438c(((u8 *)p0 + 0x5bc));
    func_0203473c(((u8 *)p0 + 0x5fc), *(u32 *)((u8 *)p0 + 0xe28), 0x38, 0, 2, *(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0xe28) + 0x18), *(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0xe28) + 0x1a));
    func_0203438c(((u8 *)p0 + 0x5fc));
    func_0203473c(((u8 *)p0 + 0x63c), *(u32 *)((u8 *)p0 + 0xe2c), 0x38, 1, 2, *(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0xe2c) + 0x18), *(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0xe2c) + 0x1a));
    func_0203438c(((u8 *)p0 + 0x63c));
}

#pragma thumb reset
