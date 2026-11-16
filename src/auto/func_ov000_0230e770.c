/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

void func_ov000_0230e6e8();
s32 func_020446b0();
s32 func_0204b7ec();

void func_ov000_0230e770(u8 *p0) {
    *(u32 *)((u8 *)p0 + 0x68) = (u32)func_ov000_0230e6e8;
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) & ~0x100);
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) & ~0x10);
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) | 0x40);
    s32 t0 = (*(u16 *)((u8 *)p0 + 0x5c) | 0x100);
    *(u16 *)((u8 *)p0 + 0x5c) = t0;
    *(u16 *)((u8 *)p0 + 0x30) = 0;
    *(u16 *)((u8 *)p0 + 0x34) = 0xa00;
    func_020446b0(4, 0xa00, 0, t0);
    *(u16 *)((u8 *)p0 + 0x62) = 0x80;
    func_0204b7ec(*(u32 *)((u8 *)p0 + 0x1dc), 0, 0x6c);
}
