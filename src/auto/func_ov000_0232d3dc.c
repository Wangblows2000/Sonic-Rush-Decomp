/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_022b4574[];
s32 func_0206b768();
extern u8 data_ov000_0236ac60[];

void func_ov000_0232d3dc(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10);
    if ((*(u32 *)((u8 *)*(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10) + 0x54) & 0x1000) != 0) {
        func_0206b768(0x8000, 0x3000, 0, t0, (t0 + 0x134), 0, 0);
    }
    s32 t1 = *(u32 *)((u8 *)t0 + 0x54);
    if ((*(u32 *)((u8 *)t0 + 0x54) & 0x2000) != 0) {
        if ((t1 & 0x4000) != 0) {
            *(u16 *)((u8 *)t0 + 0x5c) = (*(u16 *)((u8 *)t0 + 0x5c) ^ 1);
            func_0206b768(0x4800, 0x3000, 0, t0, (t0 + 0x1c4), 0, 0);
            *(u16 *)((u8 *)t0 + 0x5c) = (*(u16 *)((u8 *)t0 + 0x5c) ^ 1);
        } else {
            func_0206b768(0xb800, 0x3000, 0, t0, (t0 + 0x1c4), 0, 0);
        }
    }
    s32 t2 = (*(u32 *)((u8 *)*(u32 *)data_ov000_0236ac60 + 0x32c) & 0x400);
    if (t2 != 0) {
        *(u32 *)((u8 *)t0 + 0x54) = (*(u32 *)((u8 *)t0 + 0x54) | 4);
    }
}
