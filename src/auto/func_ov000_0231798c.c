/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_022b4574[];
s32 func_0206bd24();

void func_ov000_0231798c(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10);
    s32 t1 = *(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10);
    s32 t2 = *(u8 *)((u8 *)*(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10) + 0x1);
    s32 t3 = (*(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10) + 0x94);
    s32 t4 = (*(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10) + 0xdc);
    *(u8 *)((u8 *)t0 + 0x1) = 0;
    func_0206bd24(t0, t3, t4, 0);
    *(u8 *)((u8 *)t0 + 0x1) = t2;
    func_0206bd24(t0, (t0 + 0x20c), (t0 + 0x254));
    func_0206bd24(t0, (t0 + 0x29c), (t0 + 0x2e4));
    *(u16 *)((u8 *)t0 + 0x5c) = (*(u16 *)((u8 *)t0 + 0x5c) ^ 1);
    s32 t5 = (*(u16 *)((u8 *)t0 + 0x5c) ^ 2);
    *(u16 *)((u8 *)t0 + 0x5c) = t5;
    func_0206bd24(t0, (t0 + 0x20c), (t0 + 0x254), t5);
    func_0206bd24(t0, (t0 + 0x29c), (t0 + 0x2e4));
    *(u16 *)((u8 *)t0 + 0x5c) = (*(u16 *)((u8 *)t0 + 0x5c) ^ 1);
    *(u16 *)((u8 *)t0 + 0x5c) = (*(u16 *)((u8 *)t0 + 0x5c) ^ 2);
}
