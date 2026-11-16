/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_022faf48();
void func_ov000_022fd7d8();
extern u8 data_022c4680[];
extern u8 data_022c4560[];

void func_ov000_022fd8d8(u8 *p0) {
    func_ov000_022faf48(p0, 2);
    *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) | 0x10);
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) & ~0x10);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)func_ov000_022fd7d8;
    *(u16 *)((u8 *)p0 + 0x34) = 0;
    *(u32 *)((u8 *)p0 + 0xc) = 0x40;
    *(u32 *)((u8 *)p0 + 0x8) = 0x3c;
    *(u32 *)((u8 *)p0 + 0x1f4) = (*(u32 *)((u8 *)p0 + 0x1f4) & ~5);
    if ((*(u32 *)((u8 *)data_022c4680 + 0x8) & 1) == 0) {
        return;
    }
    if (*(u32 *)((u8 *)data_022c4560 + 0x10) == 2) {
        return;
    }
    if (*(u32 *)((u8 *)data_022c4560 + 0x10) == 3) {
        return;
    }
    *(u32 *)((u8 *)p0 + 0xc) = (*(u32 *)((u8 *)p0 + 0xc) + ((s32)*(u32 *)((u8 *)p0 + 0xc) >> 1));
    *(u32 *)((u8 *)p0 + 0x8) = (*(u32 *)((u8 *)p0 + 0x8) + ((s32)*(u32 *)((u8 *)p0 + 0xc) >> 1));
}
