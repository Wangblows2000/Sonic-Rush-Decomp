/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov000_02369acc[];
s32 func_ov000_022c71d8();
void func_ov000_022c67ec();

void func_ov000_022c6810(u8 *p0) {
    *(u16 *)((u8 *)p0 + 0x2c0) = 0;
    func_ov000_022c71d8(*(s16 *)((u8 *)(*(u32 *)data_ov000_02369acc + 0x200) + 0x28), data_ov000_02369acc, 0);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)func_ov000_022c67ec;
}
