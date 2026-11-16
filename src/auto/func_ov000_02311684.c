/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

void func_ov000_02311644();

void func_ov000_02311684(u8 *p0) {
    *(u32 *)((u8 *)p0 + 0x68) = (u32)func_ov000_02311644;
    *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) | 0x20);
    *(u16 *)((u8 *)p0 + 0x2e) = 0;
    *(u16 *)((u8 *)p0 + 0x30) = 0;
    *(u32 *)((u8 *)p0 + 0x20c) = 0;
    *(u32 *)((u8 *)p0 + 0x1b8) = 0;
    *(u32 *)((u8 *)p0 + 0x8) = 0;
    *(u32 *)((u8 *)p0 + 0x10) = (*(u32 *)((u8 *)p0 + 0x1e8) << 8);
    *(u32 *)((u8 *)p0 + 0x14) = (*(u32 *)((u8 *)p0 + 0x1ec) << 8);
}
