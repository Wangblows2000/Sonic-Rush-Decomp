/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_0203344c();
s32 func_02032d2c();

void func_ov000_02357970(s32 p0) {
    func_0203344c(((u8 *)p0 + 0x9fc), 0, 0);
    s32 t0 = func_02032d2c(((u8 *)p0 + 0x9fc));
}

#pragma thumb reset
