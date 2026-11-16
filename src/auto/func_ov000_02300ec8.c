/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_0204b7ec();
extern u8 data_ov000_0236ac60[];
s32 func_02068204();
s32 func_020446b0();
s32 func_02043f0c();
s32 func_ov000_0232e2f0();
s32 func_ov000_02343498();
void func_ov000_02300ec4();

void func_ov000_02300ec8(u8 *p0) {
    if ((*(u32 *)((u8 *)p0 + 0x54) & 4) != 0) {
        return;
    }
    func_0204b7ec(*(u32 *)((u8 *)p0 + 0x1dc), 0, 0x59);
    func_02068204(*(u32 *)((u8 *)p0 + 0x1dc), p0, *(u32 *)data_ov000_0236ac60);
    func_020446b0(4);
    func_02043f0c(0);
    func_ov000_0232e2f0(p0, 0, -32);
    func_ov000_02343498(p0, -16, -48, 0x10, 0, 8);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)func_ov000_02300ec4;
    *(u32 *)((u8 *)p0 + 0x54) = (*(u32 *)((u8 *)p0 + 0x54) | 4);
}
