/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov000_02369acc[];
s32 func_ov000_022cae4c();
s32 func_ov000_022c74e8();
s32 func_ov000_022c5f80();

void func_ov000_022cbdf4(void) {
    func_ov000_022cae4c(*(u32 *)data_ov000_02369acc);
    func_ov000_022c74e8(*(u32 *)((u8 *)*(u32 *)data_ov000_02369acc + 0x210));
    func_ov000_022c5f80(*(u32 *)((u8 *)*(u32 *)data_ov000_02369acc + 0x214));
}
