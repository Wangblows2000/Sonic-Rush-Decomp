/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_022f8fa0();
s32 func_ov000_022f0fdc();
void func_ov000_022f0750();

void func_ov000_022f07c8(u8 *p0) {
    func_ov000_022f8fa0(((u8 *)p0 + 0x450), 4, 8);
    func_ov000_022f0fdc(p0, 4);
    s32 t0 = *(u16 *)((u8 *)p0 + 0x454);
    *(u16 *)((u8 *)p0 + 0x454) = (*(u16 *)((u8 *)p0 + 0x454) + 0x1);
    if ((u32)t0 > (u32)0x1e) {
        *(u32 *)((u8 *)p0 + 0x68) = (u32)func_ov000_022f0750;
    }
}
