/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_022faf48();
void func_ov000_022fe00c();
s32 func_0206e3c0();

void func_ov000_022fe084(u8 *p0) {
    func_ov000_022faf48(p0, 2);
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) & ~0x10);
    *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) | 4);
    *(u16 *)((u8 *)p0 + 0x34) = 0;
    *(u32 *)((u8 *)p0 + 0x68) = (u32)func_ov000_022fe00c;
    *(u16 *)((u8 *)p0 + 0x238) = 2;
    func_0206e3c0(((u8 *)p0 + 0x20c), -120, -48, 0x50, -16);
}
