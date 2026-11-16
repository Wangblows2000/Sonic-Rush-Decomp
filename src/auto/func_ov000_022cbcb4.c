/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_022b4574[];
s32 func_020315e0();

void func_ov000_022cbcb4(void) {
    s32 t0 = (*(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10) + 0x23c);
    s32 t1 = (*(u16 *)((u8 *)*(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10) + 0x5c) & 0x20);
    if (t1 != 0) {
        return;
    }
    func_020315e0(t0, t1);
}
