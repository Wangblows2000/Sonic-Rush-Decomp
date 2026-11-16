/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_ov000_02360a28();
s32 func_ov000_02360ad4();
s32 func_ov000_02360ab4();
s32 func_ov000_02360a94();
s32 func_ov000_02360a4c();
s32 func_ov000_02360a04();

void func_ov000_0236119c(u8 *p0, s32 p1, s32 p2) {
    func_ov000_02360a28(p0, 0, 0);
    if (*(u32 *)((u8 *)p0 + 0x4) != 0) {
        func_ov000_02360ad4(p0, 0, 0);
    }
    func_ov000_02360ab4(p0, 0x60, 0x90);
    func_ov000_02360a94(p0, 0xfa, 0x90);
    func_ov000_02360a4c(p0, 0x48, 0xa0, p1);
    if (*(u32 *)((u8 *)p0 + 0x4) != 0) {
        if ((s32)p2 != 0) {
            func_ov000_02360a04(p0, 8, 0x90);
        }
    }
}

#pragma thumb reset
