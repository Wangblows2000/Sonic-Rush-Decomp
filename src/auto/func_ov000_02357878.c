/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_0203344c();
s32 func_02032480();
s32 func_02032d2c();

void func_ov000_02357878(s32 p0, s32 p1, s32 p2, s32 p3) {
    *(u16 *)((u8 *)p0 + 0x808) = p1;
    *(u16 *)((u8 *)p0 + 0x80a) = p2;
    func_0203344c(((u8 *)p0 + 0x804), 0, 0);
    if ((s32)p3 != 0x1000) {
        func_02032480(((u8 *)p0 + 0x804), p3, p3, 0);
        return;
    }
    func_02032d2c(((u8 *)p0 + 0x804));
}

#pragma thumb reset
