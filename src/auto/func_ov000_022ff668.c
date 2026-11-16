/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_0204b7ec();
extern u8 data_ov000_0236ac60[];
s32 func_02068204();
s32 func_ov000_0232e2f0();
s32 func_ov000_0232dedc();
s32 func_020446b0();
s32 func_02043f0c();
s32 func_ov000_02343498();

void func_ov000_022ff668(u8 *p0) {
    if ((*(u16 *)((u8 *)p0 + 0x5c) & 0x20) != 0) {
        return;
    }
    func_0204b7ec(*(u32 *)((u8 *)p0 + 0x1dc), 0, 0x59);
    func_02068204(*(u32 *)((u8 *)p0 + 0x1dc), p0, *(u32 *)data_ov000_0236ac60);
    func_ov000_0232e2f0(p0, 0, 0);
    func_ov000_0232dedc(p0, 0, -24, 0, -0x200, 4);
    func_ov000_0232dedc(p0, 0, -24, 0, -0x200, 5);
    func_020446b0(3);
    func_02043f0c(0);
    func_ov000_02343498(p0, -24, -24, 0x18, 0x18, 8);
    *(u32 *)((u8 *)p0 + 0x54) = (*(u32 *)((u8 *)p0 + 0x54) | 8);
    *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) | 0x20);
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) | 0x100);
}
