/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_0232ecac();
s32 func_ov000_0232ee24();
s32 func_ov000_0234307c();
s32 func_ov000_02336050();

void func_ov000_02342e9c(u8 *p0) {
    if (*(u8 *)((u8 *)p0 + 0x325) == 0) {
        func_ov000_0232ecac(0x32, 0x80);
    }
    func_ov000_0232ee24(p0);
    func_ov000_0234307c(p0, -0x12c0);
    func_ov000_02336050(((u32)(*(s16 *)((u8 *)p0 + 0x350) << 0xc) >> 0x10), 1);
}
