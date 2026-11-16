/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_022cc06c();
void func_ov000_022c85e8();

void func_ov000_022c825c(u8 *p0) {
    func_ov000_022cc06c(((u8 *)p0 + 0x20c), 2, 6);
    *(u16 *)((u8 *)p0 + 0x20e) = (*(u16 *)((u8 *)p0 + 0x20e) + 0x1);
    s32 t0 = *(u16 *)((u8 *)p0 + 0x20e);
    if ((u32)t0 > (u32)6) {
        *(u32 *)((u8 *)p0 + 0x68) = (u32)func_ov000_022c85e8;
    }
}
