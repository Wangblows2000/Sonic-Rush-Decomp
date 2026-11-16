/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_022faf48();
void func_ov000_022fe350();

void func_ov000_022fe7e8(u8 *p0) {
    func_ov000_022faf48(p0, 0);
    *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) | 4);
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) & ~0x10);
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) & ~0x100);
    *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) & ~0x10);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)func_ov000_022fe350;
    s32 t0 = (*(u32 *)((u8 *)p0 + 0x1f4) & 1);
    if (t0 != 0) {
        *(u32 *)((u8 *)p0 + 0xc) = 0x10;
    }
}
