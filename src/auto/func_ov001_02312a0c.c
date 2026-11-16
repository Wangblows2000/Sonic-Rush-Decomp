/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov001_0230d6f4();
s32 func_ov001_023122d8();

void func_ov001_02312a0c(u8 *p0, s32 p1) {
    func_ov001_0230d6f4(0);
    func_ov001_023122d8(p0, 0xa);
    *(u16 *)((u8 *)p0 + 0x16) = 0;
    *(u16 *)((u8 *)p0 + 0x14c) = 0;
    *(u16 *)((u8 *)p0 + 0x14e) = 1;
    *(u32 *)((u8 *)p0 + 0x128) = 1;
    *(u16 *)((u8 *)p0 + 0x150) = p1;
}
