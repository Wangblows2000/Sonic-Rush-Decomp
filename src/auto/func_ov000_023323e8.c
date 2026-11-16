/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_022b4574[];
s32 func_0206bd24();

void func_ov000_023323e8(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10);
    func_0206bd24(*(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10), (*(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10) + 0x94), (*(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10) + 0xdc));
    s32 t1 = *(u16 *)((u8 *)t0 + 0x5c);
    *(u16 *)((u8 *)t0 + 0x5c) = (*(u16 *)((u8 *)t0 + 0x5c) ^ 2);
    s32 t2 = (*(u16 *)((u8 *)t0 + 0x5c) | 0x1000);
    *(u16 *)((u8 *)t0 + 0x5c) = t2;
    func_0206bd24(t0, (t0 + 0x94), (t0 + 0xdc), t2);
    *(u16 *)((u8 *)t0 + 0x5c) = t1;
}
