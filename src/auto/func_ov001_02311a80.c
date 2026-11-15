/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov001_023122d8();
s32 func_ov001_02312aac();
s32 func_ov001_0230d6f4();

void func_ov001_02311a80(u8 *p0) {
    s32 t0 = (*(u16 *)((u8 *)p0 + 0x64) | 0x2000);
    *(u16 *)((u8 *)p0 + 0x64) = t0;
    func_ov001_023122d8(p0, 2, t0);
    func_ov001_02312aac(p0);
    s32 t1 = *(u16 *)((u8 *)p0 + 0x16);
    *(u32 *)((u8 *)p0 + 0x134) = t1;
    s32 t2 = func_ov001_0230d6f4(0x3000, t1);
}
