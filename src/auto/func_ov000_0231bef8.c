/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

void func_ov000_0231bec0();
s32 func_0201c070();
s32 func_0204b7ec();
extern u8 data_ov000_0236ac60[];
s32 func_02068204();

void func_ov000_0231bef8(u8 *p0) {
    *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) & ~0x10);
    s32 t0 = (*(u16 *)((u8 *)p0 + 0x5c) | 4);
    *(u16 *)((u8 *)p0 + 0x5c) = t0;
    *(u32 *)((u8 *)p0 + 0xc) = 0x50;
    *(u32 *)((u8 *)p0 + 0x68) = (u32)func_ov000_0231bec0;
    func_0201c070(*(u32 *)((u8 *)p0 + 0x1dc), 0, 0x50, t0);
    func_0204b7ec(*(u32 *)((u8 *)p0 + 0x1dc), 0, 0x56);
    func_02068204(*(u32 *)((u8 *)p0 + 0x1dc), p0, *(u32 *)data_ov000_0236ac60);
}
