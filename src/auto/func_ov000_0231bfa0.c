/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_02069818();
void func_ov000_0231bf68();

void func_ov000_0231bfa0(u8 *p0) {
    func_02069818(p0, 0);
    *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) & ~4);
    *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) | 0x10);
    *(u32 *)((u8 *)p0 + 0xc) = ((u32)((*(u8 *)((u8 *)*(u32 *)((u8 *)p0 + 0x1e0) + 0x8) * 0x64 + *(u8 *)((u8 *)*(u32 *)((u8 *)p0 + 0x1e0) + 0x9)) << 0x10) >> 0x10);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)func_ov000_0231bf68;
}
