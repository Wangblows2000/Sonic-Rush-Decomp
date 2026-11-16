/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_022b4574[];
s32 func_0206bd24();

void func_ov000_02304704(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10);
    func_0206bd24(*(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10), (*(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10) + 0x94), (*(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10) + 0xdc));
    func_0206bd24(t0, (t0 + 0x20c), (t0 + 0x254));
    *(u16 *)((u8 *)t0 + 0x5c) = (*(u16 *)((u8 *)t0 + 0x5c) | 0x100);
    func_0206bd24(t0, (t0 + 0x29c), (t0 + 0x2e4));
    *(u16 *)((u8 *)t0 + 0x5c) = (*(u16 *)((u8 *)t0 + 0x5c) & ~0x100);
}
