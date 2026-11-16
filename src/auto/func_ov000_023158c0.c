/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_02069818();
void func_ov000_023156e0();
s32 func_0204b7ec();

void func_ov000_023158c0(u8 *p0) {
    func_02069818(p0, 1);
    *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) | 4);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)func_ov000_023156e0;
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) & ~0x2000);
    *(u32 *)((u8 *)p0 + 0xc) = 0;
    *(u16 *)((u8 *)p0 + 0x34) = 0;
    *(u16 *)((u8 *)p0 + 0x34) = 0;
    *(u32 *)((u8 *)p0 + 0x8) = 0x100;
    *(u16 *)((u8 *)p0 + 0x62) = 0x60;
    func_0204b7ec(*(u32 *)((u8 *)p0 + 0x1dc), 0, 0x6c);
}
