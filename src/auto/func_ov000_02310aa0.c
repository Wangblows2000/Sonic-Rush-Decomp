/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_02069818();
void func_ov000_02310a68();

void func_ov000_02310aa0(u8 *p0) {
    func_02069818(p0, *(u16 *)((u8 *)p0 + 0x1f8));
    *(u32 *)((u8 *)p0 + 0x68) = (u32)func_ov000_02310a68;
    *(u32 *)((u8 *)p0 + 0xc) = 0;
    *(u32 *)((u8 *)p0 + 0x8) = 0;
    *(u32 *)((u8 *)p0 + 0x54) = (*(u32 *)((u8 *)p0 + 0x54) | 2);
    *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) & ~0x20);
    *(u32 *)((u8 *)p0 + 0x8) = 0x18;
    *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) | 0x10);
}
