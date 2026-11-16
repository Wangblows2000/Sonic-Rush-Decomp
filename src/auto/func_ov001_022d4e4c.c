/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_0203473c();
s32 func_0203438c();

void func_ov001_022d4e4c(s32 p0) {
    func_0203473c(((u8 *)p0 + 0xdc), *(u32 *)((u8 *)p0 + 0x168), 0x38, 0, 3, *(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0x168) + 0x18), *(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0x168) + 0x1a));
    func_0203438c(((u8 *)p0 + 0xdc));
    func_0203473c(((u8 *)p0 + 0x11c), *(u32 *)((u8 *)p0 + 0x168), 0x38, 1, 3, *(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0x168) + 0x18), *(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0x168) + 0x1a));
    func_0203438c(((u8 *)p0 + 0x11c));
    func_0203473c(((u8 *)p0 + 0x9c), *(u32 *)((u8 *)p0 + 0x164), 0x38, 0, 2, *(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0x164) + 0x18), *(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0x164) + 0x1a));
    func_0203438c(((u8 *)p0 + 0x9c));
}

#pragma thumb reset
