/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_0203344c();
s32 func_02032d2c();

void func_ov000_023579fc(s32 p0, s32 p1, s32 p2) {
    *(u16 *)((u8 *)p0 + 0x610) = p1;
    *(u16 *)((u8 *)p0 + 0x612) = p2;
    func_0203344c(((u8 *)p0 + 0x60c), 0, 0);
    func_02032d2c(((u8 *)p0 + 0x60c));
}

#pragma thumb reset
