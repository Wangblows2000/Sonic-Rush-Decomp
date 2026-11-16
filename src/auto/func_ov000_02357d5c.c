/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 data_022b4574[];
s32 func_02035704();

void func_ov000_02357d5c(void) {
    s32 t0 = *(u32 *)*(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10);
    s32 t1 = *(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10);
    s32 t2 = (*(u32 *)*(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10) & 3);
    if (t2 != 0) {
        *(u32 *)t1 = (t0 + 0x1);
        return;
    }
    func_02035704(((t1 + 0xc) + (((u32)((u32)t0 % (u32)0x30) >> 2) << 5)), 0x10, *(u32 *)((u8 *)*(u32 *)((u8 *)t1 + 0x4) + 0x14), *(u32 *)((u8 *)*(u32 *)((u8 *)t1 + 0x4) + 0x18));
    func_02035704(((t1 + 0x20c) + (((u32)((u32)t0 % (u32)0x30) >> 2) << 5)), 0x10, *(u32 *)((u8 *)*(u32 *)((u8 *)t1 + 0x8) + 0x14), *(u32 *)((u8 *)*(u32 *)((u8 *)t1 + 0x8) + 0x18));
    *(u32 *)t1 = (*(u32 *)t1 + 0x1);
}

#pragma thumb reset
