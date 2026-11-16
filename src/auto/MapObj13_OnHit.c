/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_02343228();
s32 MapObj13_CreatePart_ov000_0230bc50();

void MapObj13_OnHit(u8 *p0, u8 *p1) {
    u32 t0 = *(u32 *)((u8 *)p0 + 0x18);
    u32 t1 = *(u32 *)((u8 *)p1 + 0x18);
    if (t0 == 0) {
        return;
    }
    if (t1 == 0) {
        u8 t2 = *(u8 *)t1;
        if (t2 != 1) {
            return;
        }
    }
    u32 t3 = *(u32 *)((u8 *)t0 + 0x54);
    if ((t3 & 2) != 0) {
        return;
    }
    func_ov000_02343228(t1, 1);
    u32 t4 = *(u32 *)((u8 *)t0 + 0x54);
    *(u32 *)((u8 *)t0 + 0x54) = (t4 | 8);
    u32 t5 = *(u32 *)((u8 *)t0 + 0x54);
    *(u32 *)((u8 *)t0 + 0x54) = (t5 | 2);
    u16 t6 = *(u16 *)((u8 *)t0 + 0x5c);
    *(u16 *)((u8 *)t0 + 0x5c) = (t6 | 0x20);
    MapObj13_CreatePart_ov000_0230bc50(t0, (t6 | 0x20));
}
