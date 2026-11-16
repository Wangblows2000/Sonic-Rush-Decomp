/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_022b4574[];
s32 func_020316f4();
s32 func_020315e0();

void func_ov000_022da3b4(void) {
    if ((*(u16 *)((u8 *)*(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10) + 0x5c) & 0x20) != 0) {
        return;
    }
    s32 t0 = (*(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10) + 0x23c);
    func_020316f4((*(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10) + 0x23c), *(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10));
    func_020315e0(t0);
}
