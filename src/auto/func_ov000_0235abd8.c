/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_022b4574[];
s32 func_020371c8();
s32 func_020316f4();
s32 func_020315e0();

void func_ov000_0235abd8(void) {
    s32 t0 = *(u32 *)data_022b4574;
    s32 t1 = *(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10);
    s32 t2 = (*(u32 *)((u8 *)*(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10) + 0xbc) & 2);
    if (t2 != 0) {
        func_020371c8(t0, t2);
        return;
    }
    func_020316f4(t1);
    func_020315e0(t1);
}
