/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_022faf48();
void func_ov000_022fea7c();
s32 EffEntrance_Create();

void func_ov000_022fec38(u8 *p0) {
    func_ov000_022faf48(p0, 0);
    *(u32 *)((u8 *)p0 + 0xc0) = (*(u32 *)((u8 *)p0 + 0xc0) | 0x200);
    *(u32 *)((u8 *)p0 + 0x108) = (*(u32 *)((u8 *)p0 + 0x108) | 0x200);
    *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) | 0x10);
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) | 0x2000);
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) & ~0x80);
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) | 0x100);
    *(u32 *)((u8 *)p0 + 0x54) = (*(u32 *)((u8 *)p0 + 0x54) | 2);
    *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) | 0x20);
    *(u16 *)((u8 *)p0 + 0x44) = 8;
    *(u16 *)((u8 *)p0 + 0x46) = 0x180;
    *(u32 *)((u8 *)p0 + 0xc) = 0x18;
    *(u32 *)((u8 *)p0 + 0x8) = 0;
    *(u32 *)((u8 *)p0 + 0x68) = (u32)func_ov000_022fea7c;
    EffEntrance_Create(p0, 0, -16, (u32)func_ov000_022fea7c);
}
