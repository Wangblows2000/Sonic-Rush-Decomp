/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_022b4574[];
s32 func_02044798();
extern u8 data_ov000_02369ac0[];
void func_0204481c();
s32 func_02003d78();
extern u8 data_02072c78[];

void func_02044ad0(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10) + 0x4);
    s32 t1 = *(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10);
    *(u16 *)((u8 *)t1 + 0xa) = (*(u16 *)((u8 *)*(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10) + 0xa) + 0x1);
    s32 t2 = *(u16 *)((u8 *)t1 + 0xa);
    if ((u32)t2 > (u32)2) {
        *(u16 *)((u8 *)t1 + 0xc) = (*(u16 *)((u8 *)t1 + 0xc) ^ 1);
        *(u16 *)((u8 *)t1 + 0xa) = 0;
    }
    func_02044798(t0, *(u16 *)((u8 *)t1 + 0x8), ((u32)((*(u16 *)((u8 *)t1 + 0xc) + 0x6) << 0x10) >> 0x10));
    if (*(u16 *)((u8 *)t1 + 0x8) == *(u16 *)((u8 *)data_ov000_02369ac0 + 0x8)) {
        return;
    }
    *(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x8) = (u32)func_0204481c;
    *(u32 *)t1 = 0;
    func_02003d78(0x4001050, 0, 4, 0x1f, 0);
    s32 t3 = *(u32 *)((u8 *)data_02072c78 + 0x4);
    *(u16 *)((u8 *)t3 + 0x20) = 0x440;
    *(u16 *)((u8 *)t3 + 0x22) = 0x1f;
}
