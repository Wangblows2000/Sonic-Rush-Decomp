/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_0203473c();
s32 func_0203438c();

void func_ov001_022d6b5c(s32 p0) {
    func_0203473c(((u8 *)p0 + 0x3cc), *(u32 *)((u8 *)p0 + 0x498), 0x38, 0, 2, *(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0x498) + 0x18), *(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0x498) + 0x1a));
    func_0203438c(((u8 *)p0 + 0x3cc));
    func_0203473c(((u8 *)p0 + 0x40c), *(u32 *)((u8 *)p0 + 0x49c), 0x38, 0, 3, *(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0x49c) + 0x18), *(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0x49c) + 0x1a));
    func_0203438c(((u8 *)p0 + 0x40c));
    func_0203473c(((u8 *)p0 + 0x44c), *(u32 *)((u8 *)p0 + 0x4a0), 0x38, 1, 3, *(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0x4a0) + 0x18), *(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0x4a0) + 0x1a));
    func_0203438c(((u8 *)p0 + 0x44c));
}

#pragma thumb reset
