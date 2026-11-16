/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_0201c070();
s32 func_0204b7ec();
s32 func_ov000_0232e2f0();
s32 func_ov000_0232dedc();
s32 func_020446b0();
s32 func_02043f0c();
s32 func_ov000_02343498();

void func_ov000_02313bb0(u8 *p0) {
    if ((*(u16 *)((u8 *)p0 + 0x5c) & 0x20) != 0) {
        return;
    }
    func_0201c070(*(u32 *)((u8 *)p0 + 0x1dc), 0);
    func_0204b7ec(0, 0, 0x64);
    func_ov000_0232e2f0(p0, 0, 0);
    func_ov000_0232dedc(p0, 0, -24, 0, -0x200, 4);
    func_ov000_0232dedc(p0, 0, -24, 0, -0x200, 5);
    func_ov000_0232dedc(p0, 0, -24, 0, -0x200, 6);
    func_ov000_0232dedc(p0, 0, -24, 0, -0x200, 7);
    func_020446b0(4);
    func_02043f0c(0);
    s32 t0 = func_ov000_02343498(p0, -48, -48, 0x30, 0x30, 8);
    if (t0 != 0) {
        *(u8 *)((u8 *)t0 + 0x188) = 0x21;
    }
    *(u32 *)((u8 *)p0 + 0x54) = (*(u32 *)((u8 *)p0 + 0x54) | 8);
    *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) | 0x20);
}
