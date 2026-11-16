/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov001_0231599c();
s32 func_ov001_023122d8();
s32 func_ov001_0230d6f4();

void func_ov001_02311968(u8 *p0) {
    *(u32 *)((u8 *)p0 + 0x74) = 0xb4;
    func_ov001_0231599c(0, 0xb4);
    func_ov001_023122d8(p0, 2);
    *(u32 *)((u8 *)p0 + 0x120) = 0;
    s32 t0 = *(u16 *)((u8 *)p0 + 0x16);
    *(u32 *)((u8 *)p0 + 0x134) = t0;
    func_ov001_0230d6f4(0x3000, t0);
}
