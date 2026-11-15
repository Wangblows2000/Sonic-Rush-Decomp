/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_02307e84();
s32 func_ov000_0234237c();
void func_ov000_023086a8();

void func_ov000_02308720(u8 *p0) {
    s32 t0 = func_ov000_02307e84();
    func_ov000_0234237c(1, t0);
    s32 t1 = *(u32 *)((u8 *)p0 + 0x14);
    *(u32 *)((u8 *)p0 + 0x20c) = *(u32 *)((u8 *)p0 + 0x10);
    *(u32 *)((u8 *)p0 + 0x210) = t1;
    *(u32 *)((u8 *)p0 + 0x68) = (u32)func_ov000_023086a8;
}
