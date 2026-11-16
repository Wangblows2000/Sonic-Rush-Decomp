/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_ov001_022e4e64();
s32 func_ov001_022e5620();
s32 func_ov001_022e3b30();
s32 func_ov001_022e36c4();

void func_ov001_022e8528(s32 p0) {
    func_ov001_022e4e64(((u8 *)p0 + 0x674));
    func_ov001_022e5620(((u8 *)p0 + 0x4));
    func_ov001_022e3b30(((u8 *)p0 + 0x1994));
    func_ov001_022e36c4(((u8 *)p0 + 0x1b08));
}

#pragma thumb reset
