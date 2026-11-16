/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_0234c9e8();
s32 func_ov000_0234c4a4();
void func_ov000_02352044();
extern u8 data_ov000_02368ba0[];
s32 func_0204b7ec();

void func_ov000_023521f0(u8 *p0) {
    func_ov000_0234c9e8();
    func_ov000_0234c4a4(p0, 0x2d);
    *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) | 4);
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) & ~0x10);
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) & ~0x8000);
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) & ~0x40000);
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) | 0x200);
    *(u32 *)((u8 *)p0 + 0x32c) = (*(u32 *)((u8 *)p0 + 0x32c) | 0x100000);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)func_ov000_02352044;
    *(u16 *)((u8 *)p0 + 0x3c) = *(s16 *)((u8 *)data_ov000_02368ba0 + (*(u8 *)((u8 *)p0 + 0x324) * 0x30));
    *(u8 *)((u8 *)p0 + 0x4) = 0;
    func_0204b7ec(((u8 *)p0 + 0x384), 0, 0x5c);
}
