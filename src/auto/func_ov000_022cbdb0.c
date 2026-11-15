/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov000_02369acc[];
s32 func_ov000_022cac48();
s32 func_ov000_022c74d4();
s32 func_ov000_022c5f6c();

void func_ov000_022cbdb0(void) {
    func_ov000_022cac48(*(u32 *)data_ov000_02369acc);
    func_ov000_022c74d4(*(u32 *)((u8 *)*(u32 *)data_ov000_02369acc + 0x210));
    s32 t0 = func_ov000_022c5f6c(*(u32 *)((u8 *)*(u32 *)data_ov000_02369acc + 0x214));
}
