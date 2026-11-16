/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov000_02369acc[];
s32 func_ov000_022cb12c();
s32 func_ov000_022c9da8();
s32 func_ov000_022c696c();

void func_ov000_022cbfc8(void) {
    func_ov000_022cb12c(*(u32 *)data_ov000_02369acc);
    func_ov000_022c9da8(*(u32 *)((u8 *)*(u32 *)data_ov000_02369acc + 0x210));
    func_ov000_022c696c(*(u32 *)((u8 *)*(u32 *)data_ov000_02369acc + 0x214));
}
