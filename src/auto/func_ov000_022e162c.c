/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_022b4574[];
s32 func_020316f4();
s32 func_020315e0();

void func_ov000_022e162c(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10);
    s32 t1 = (*(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10) + 0x20c);
    *(u32 *)((u8 *)t1 + 0x48) = (*(u32 *)((u8 *)*(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10) + 0x10) << 4);
    *(u32 *)((u8 *)t1 + 0x4c) = (-*(u32 *)((u8 *)t0 + 0x14) << 4);
    s32 t2 = (*(u32 *)((u8 *)t0 + 0x18) << 4);
    *(u32 *)((u8 *)t1 + 0x50) = t2;
    func_020316f4(t1, t2, t0);
    func_020315e0(t1);
}
