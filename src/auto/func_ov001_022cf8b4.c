/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_0203473c();
s32 func_0203438c();

void func_ov001_022cf8b4(s32 p0) {
    func_0203473c(((u8 *)p0 + 0x8), *(u32 *)((u8 *)p0 + 0x88), 0x38, 0, 3, *(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0x88) + 0x18), *(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0x88) + 0x1a));
    func_0203438c(((u8 *)p0 + 0x8));
    func_0203473c(((u8 *)p0 + 0x48), *(u32 *)((u8 *)p0 + 0x8c), 0x38, 1, 3, *(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0x8c) + 0x18), *(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0x8c) + 0x1a));
    func_0203438c(((u8 *)p0 + 0x48));
}

#pragma thumb reset
