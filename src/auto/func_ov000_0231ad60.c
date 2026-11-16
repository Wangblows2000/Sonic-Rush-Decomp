/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_0231a94c();
void func_ov000_0231ac3c();

void func_ov000_0231ad60(u8 *p0) {
    func_ov000_0231a94c();
    *(u32 *)((u8 *)p0 + 0x54) = (*(u32 *)((u8 *)p0 + 0x54) & ~2);
    *(u32 *)((u8 *)p0 + 0xc) = ((u32)((*(u8 *)((u8 *)*(u32 *)((u8 *)p0 + 0x1e0) + 0x8) * 0x64 + *(u8 *)((u8 *)*(u32 *)((u8 *)p0 + 0x1e0) + 0x9)) << 0x10) >> 0x10);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)func_ov000_0231ac3c;
}
