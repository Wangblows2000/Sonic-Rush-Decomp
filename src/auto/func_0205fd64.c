/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_020075b8();
s32 func_02007450();

void func_0205fd64(s32 p0) {
    func_020075b8();
    func_02007450(((u8 *)p0 + 0x4c));
    func_02007450(((u8 *)p0 + 0x74));
    func_02007450(((u8 *)p0 + 0x9c));
    func_02007450(((u8 *)p0 + 0xc4));
    s32 t0 = func_02007450(((u8 *)p0 + 0x24));
}

#pragma thumb reset
