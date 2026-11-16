/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_0234aa80();
s32 func_020446b0();
s32 func_ov000_023136ec();
s32 func_0204b7ec();

void func_ov000_02313854(u8 *p0, u8 *p1) {
    u32 t0 = *(u32 *)((u8 *)p0 + 0x18);
    u32 t1 = *(u32 *)((u8 *)p1 + 0x18);
    if (t1 == 0) {
        return;
    }
    u32 t2 = *(u32 *)((u8 *)t1 + 0x58);
    if ((t2 & 0x10) == 0) {
        return;
    }
    func_ov000_0234aa80(t1, (t2 & 0x10));
    func_020446b0(3);
    u32 t3 = *(u32 *)((u8 *)t0 + 0x54);
    *(u32 *)((u8 *)t0 + 0x54) = (t3 | 8);
    u32 t4 = *(u32 *)((u8 *)t0 + 0x54);
    *(u32 *)((u8 *)t0 + 0x54) = (t4 | 2);
    u32 t5 = *(u32 *)((u8 *)t0 + 0x1f4);
    *(u32 *)((u8 *)t0 + 0x1f4) = (t5 | 0x10);
    u16 t6 = *(u16 *)((u8 *)(t0 + 0x100) + 0x3e);
    *(u16 *)((u8 *)(t0 + 0x100) + 0x3e) = (t6 | 0x1000);
    u16 t7 = *(u16 *)((u8 *)(t0 + 0x100) + 0x6a);
    *(u16 *)((u8 *)(t0 + 0x100) + 0x6a) = (t7 | 0x1000);
    func_ov000_023136ec(t0, 0x300, -0x200, 0);
    func_ov000_023136ec(t0, -0x200, -0x280, 0);
    func_ov000_023136ec(t0, -0x330, -0x2e0, 1);
    func_ov000_023136ec(t0, 0x240, -0x320, 1);
    func_0204b7ec(0, 0, 0x4c);
}
