/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

void func_ov000_0230f280();
void func_ov000_0230f1d8();
s32 func_0204b7ec();
extern u8 data_ov000_0236ac60[];
s32 func_02068204();

void func_ov000_0230f474(u8 *p0) {
    *(u32 *)((u8 *)p0 + 0x68) = (u32)func_ov000_0230f280;
    *(u32 *)((u8 *)p0 + 0x70) = (u32)func_ov000_0230f1d8;
    *(u32 *)((u8 *)p0 + 0xc) = 0;
    func_0204b7ec(*(u32 *)((u8 *)p0 + 0x1dc), 0, 0x4b);
    func_02068204(*(u32 *)((u8 *)p0 + 0x1dc), p0, *(u32 *)data_ov000_0236ac60);
}
