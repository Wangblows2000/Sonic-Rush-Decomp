/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

void func_ov000_022ffdac();

void func_ov000_022ffef8(u8 *p0) {
    *(u32 *)((u8 *)p0 + 0x68) = (u32)func_ov000_022ffdac;
    *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) ^ 1);
    *(u32 *)((u8 *)p0 + 0x1f4) = (*(u32 *)((u8 *)p0 + 0x1f4) & ~1);
    *(u16 *)((u8 *)p0 + 0x34) = 0;
    *(u16 *)((u8 *)p0 + 0x2e) = 0;
    *(u16 *)((u8 *)p0 + 0x30) = 0;
    *(u32 *)((u8 *)p0 + 0x8) = 0;
    *(u32 *)((u8 *)p0 + 0x1fc) = *(u32 *)((u8 *)p0 + 0x10);
    *(u32 *)((u8 *)p0 + 0x200) = *(u32 *)((u8 *)p0 + 0x14);
}
